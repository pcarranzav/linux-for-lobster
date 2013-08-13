/*
 * wahoo.c -- Support for Wahoo custom acquisition device.
 *
 * Copyright (c) 2013 Grupo 1
 *	Pablo Carranza Velez, <pablocarranza@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include <linux/spi/spi.h>
#include <linux/spi/wahoo.h>

#define RX_BUFSIZE 1024
#define TX_BUFSIZE 4

#define RESPONSE_OK 0x01

#define CHANNEL0 0x00;
#define CHANNEL1 0x01;

// wahoo commands
#define CMD_HELLO					0x01

#define CMD_CHANNEL_DATAREQ	0x80
#define CMD_CHANNEL_CONF		0x42
#define CMD_CHANNEL_OFFSET		0x26

#define CMD_TRIGGER_CONF		0x12
#define CMD_TRIGGER_LEVEL		0x13
#define CMD_TRIGGER_HOLDOFF	0x0B

#define CMD_SAMPLE_RATE			0x0A
#define CMD_RESET					0xF8

// TRIGGER_CONF bit masks and offsets
#define TR_CHANNEL_OF			5
#define TR_CHANNEL_MASK			0x20
#define TR_ROLL_OF				4
#define TR_ROLL_MASK				0x10
#define TR_AUTO_OF				3
#define TR_AUTO_MASK				0x08
#define TR_SLOPE_OF				2
#define TR_SLOPE_MASK			0x04
#define TR_NOISE_OF				1
#define TR_NOISE_MASK			0x02
#define TR_HF_OF					0
#define TR_HF_MASK				0x01				

// CHANNEL_CONF bit masks and offsets
#define CH_ON_OF					5
#define CH_ON_MASK				0x20
#define CH_COUPLING_OF			4
#define CH_COUPLING_MASK		0x10
#define CH_GAIN_OF				0
#define CH_GAIN_MASK				0x0F

#define SAMPLE_RATE_MASK 		0x1F
#define TR_HOLDOFF_MASK 		0x1F

struct wahoo_state {
	struct spi_device	*us;
	struct mutex		lock;
	
	struct chan channel[2];
	
	struct trig trigger;

	unsigned char sampleRate;
	unsigned char	*	tx_buff;
	unsigned char	*	rx_buff;
};


static unsigned char to_channel_id(struct device_attribute *attr);

// Send a command to Wahoo. Returns the one-byte response or a negative errno.
static int wahoo_send_command(struct wahoo_state *st,unsigned char cmd,unsigned char ops[], int nops)
{
	unsigned char *buf = st->tx_buff;
	int i;
	int ret;
	unsigned char val=0;
	buf[0] = cmd;
	for(i=0;i<nops;i++)
		buf[i+1] = ops[i];
	
	
	ret =  spi_write_then_read(st->us, buf, nops+1, &val, 1);
	//dev_err(&st->us->dev, "Received %x\n",val);
	
	if(ret<0)
	{	
		dev_err(&st->us->dev, " Error: SPI fail");
		return ret;
	}
	else
	{
		if(val!=(unsigned char)1)
			dev_err(&st->us->dev, "Error: Received %x\n instead of 1",val);
		return val;	
	}
}

static ssize_t wahoo_trigger_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	int i;

	mutex_lock(&st->lock);
	
	for (i=0;i<sizeof(struct trig);i++)
		buf[i] = *(((char*)&st->trigger)+i);
	
	mutex_unlock(&st->lock);

	return sizeof(struct trig);
}

static ssize_t wahoo_trigger_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	struct trig *tr = (struct trig *) buf;
	unsigned char ops[3];
	int ret;
	if (len != sizeof(struct trig))
		return -EINVAL;


	mutex_lock(&st->lock);
	
	//if ((tr->channel != st->trigger.channel) || (tr->slope != st->trigger.slope) || 
	//	(tr->aut != st->trigger.aut) || (tr->roll != st->trigger.roll) || (tr->noise != st->trigger.noise) || 
	//	(tr->hf != st->trigger.hf) )
	//{
		//Send trigger cmd.
		ops[0] = ((tr->channel << TR_CHANNEL_OF) & TR_CHANNEL_MASK) | ((tr->slope << TR_SLOPE_OF) & TR_SLOPE_MASK) |
					((tr->aut  << TR_AUTO_OF) & TR_AUTO_MASK) | ((tr->roll << TR_ROLL_OF) & TR_ROLL_MASK) |
					((tr->noise << TR_NOISE_OF) & TR_NOISE_MASK) | ((tr->hf << TR_HF_OF) & TR_HF_MASK);
					
		ret = wahoo_send_command(st,CMD_TRIGGER_CONF,ops,1);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//if(tr->level != st->trigger.level)
	//{
		ops[0] = tr->level;
		ret = wahoo_send_command(st,CMD_TRIGGER_LEVEL,ops,1);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//if(tr->holdOff != st->trigger.holdOff)
	//{
		ops[0] = tr->holdOff & TR_HOLDOFF_MASK;
		ret = wahoo_send_command(st,CMD_TRIGGER_HOLDOFF,ops,1);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//Save the new trigger configuration
	memcpy(&(st->trigger),buf,sizeof(struct trig));
	
	mutex_unlock(&st->lock);

	return len;
}

static inline int wahoo_read_buf(struct wahoo_state *st,char * buf, int len)
{
	struct spi_transfer xfer = {
		.rx_buf		= buf,
		.len		=  len,
	};
	struct spi_message msg;

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);

	return spi_sync(st->us, &msg);
}



static ssize_t wahoo_channel_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	unsigned char ch = to_channel_id(attr);
	int i;

	if(ch == 0xFF)
		return -EINVAL;

	mutex_lock(&st->lock);
	
	for (i=0;i<sizeof(struct chan);i++)
		buf[i] = *(((char*)&st->channel[ch])+i);
	
	mutex_unlock(&st->lock);

	return sizeof(struct chan);
}

static ssize_t wahoo_channel_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	struct chan *cha = (struct chan *) buf;
	unsigned char ops[3];
	unsigned char ch = to_channel_id(attr);
	int ret;
	
	//dev_err(&st->us->dev, "Channel data store: %x %x\n",ch,cha->id);
	if(ch == 0xFF)
		return -EINVAL;

	if (len != sizeof(struct chan))
		return -EINVAL;

	if(ch != cha->id)
		return -EINVAL;


	mutex_lock(&st->lock);
	
	//if ((cha->on != st->channel[ch].on) || (cha->gain != st->channel[ch].gain) || (cha->coupling != st->channel[ch].coupling) )
	//{
		//Send CHANNEL_CONF cmd.
		ops[0] = ((cha->on << CH_ON_OF) & CH_ON_MASK) | ((cha->coupling << CH_COUPLING_OF) & CH_COUPLING_MASK) |
					((cha->gain << CH_GAIN_OF) & CH_GAIN_MASK);
					
		ret = wahoo_send_command(st,CMD_CHANNEL_CONF|ch,ops,1);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//if(cha->offset != st->channel[ch].offset)
	//{
		ops[0] = (unsigned char)((cha->offset >> 16) & 0xFF) ;
		ops[1] = (unsigned char)((cha->offset >> 8) & 0xFF) ;
		ops[2] = (unsigned char)((cha->offset) & 0xFF) ;
		
		ret = wahoo_send_command(st,CMD_CHANNEL_OFFSET|ch,ops,3);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//Save the new channel configuration
	memcpy(&(st->channel[ch]),buf,sizeof(struct chan));
	
	mutex_unlock(&st->lock);

	return len;
}


static ssize_t wahoo_channel_data_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	unsigned char ch = to_channel_id(attr);

	int ret;
	unsigned char nBuf[2] = {0,0};
	unsigned int length;
	
	if(ch == 0xFF)
		return -EINVAL;


	mutex_lock(&st->lock);
	
	ret = wahoo_send_command(st,CMD_CHANNEL_DATAREQ|ch,NULL,0);
	if(ret<0)
	{
		//dev_err(&st->us->dev, "Ret: %x\n",ret);
		mutex_unlock(&st->lock);
		return ret;
	}
	else if(ret!=RESPONSE_OK)
	{
		//dev_err(&st->us->dev, "Ret: %x\n",ret);
		mutex_unlock(&st->lock);
		return -EREMOTEIO;
	}
	
	ret = wahoo_read_buf(st,(char*)nBuf,2);
	if(ret<0)
	{
		mutex_unlock(&st->lock);
		return ret;
	}
	
	length = ((nBuf[0] << 8) & 0xFF00) + nBuf[1];
	//dev_err(&st->us->dev, "Len: %d\n",length);
	if(length)
	{
		ret = wahoo_read_buf(st,st->rx_buff,length);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
	
		memcpy(buf,st->rx_buff,length);
	}
	mutex_unlock(&st->lock);

	//sysfs_update_file()?
	return length;
}


static ssize_t wahoo_sample_rate_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct wahoo_state *st = dev_get_drvdata(dev);


	mutex_lock(&st->lock);

	buf[0] = (char) (st->sampleRate & SAMPLE_RATE_MASK);
	
	mutex_unlock(&st->lock);

	return 1;
}

static ssize_t wahoo_sample_rate_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	unsigned char ops[3];
	int ret;
	
	if (len != 1)
		return -EINVAL;

	mutex_lock(&st->lock);
	
	//if ( ((unsigned char)(buf[0] & SAMPLE_RATE_MASK)) != st->sampleRate)
	//{
		//Send CHANNEL_CONF cmd.
		ops[0] = (unsigned char) (buf[0] & SAMPLE_RATE_MASK);
					
		ret = wahoo_send_command(st,CMD_SAMPLE_RATE,ops,1);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	//}
	
	//Save the new sample rate configuration
	st->sampleRate = (unsigned char) (buf[0] & SAMPLE_RATE_MASK);
	
	mutex_unlock(&st->lock);

	return len;
}

static ssize_t wahoo_reset(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
	struct wahoo_state *st = dev_get_drvdata(dev);
	int ret;
	
	if (len != 1)
		return -EINVAL;

	mutex_lock(&st->lock);
	
	if (buf[0])
	{
		
					
		ret = wahoo_send_command(st,CMD_RESET,NULL,0);
		
		if(ret<0)
		{
			mutex_unlock(&st->lock);
			return ret;
		}
		else if(ret!=RESPONSE_OK)
		{
			mutex_unlock(&st->lock);
			return -EREMOTEIO;
		}
	}
	
	st->channel[0].id = 0;
	st->channel[0].on = 0;
	st->channel[0].gain = 1;
	st->channel[0].coupling = 0;
	st->channel[0].offset = 0;
	
	st->channel[1].id = 1;
	st->channel[1].on = 0;
	st->channel[1].gain = 1;
	st->channel[1].coupling = 0;
	st->channel[1].offset = 0;
	
	st->trigger.channel = 0;
	st->trigger.slope = 1;
	st->trigger.aut = 0;
	st->trigger.roll = 0;
	st->trigger.noise = 0;
	st->trigger.hf = 0;
	st->trigger.holdOff = 0;
	st->trigger.level = 0;
	
	mutex_unlock(&st->lock);

	return len;
}

static ssize_t wahoo_read_reset(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct wahoo_state *st = dev_get_drvdata(dev);


	mutex_lock(&st->lock);

	buf[0] = 0;
	
	mutex_unlock(&st->lock);

	return 1;
}

static DEVICE_ATTR(trigger, S_IWUSR|S_IRUGO,
		wahoo_trigger_show, wahoo_trigger_store);
		
static DEVICE_ATTR(channelAstate, S_IWUSR|S_IRUGO,
		wahoo_channel_show, wahoo_channel_store);
		
static DEVICE_ATTR(channelBstate, S_IWUSR|S_IRUGO,
		wahoo_channel_show, wahoo_channel_store);

static DEVICE_ATTR(sampleRate, S_IWUSR |S_IRUGO,
		wahoo_sample_rate_show, wahoo_sample_rate_store);

static DEVICE_ATTR(channelAdata, S_IRUGO,
		wahoo_channel_data_show, NULL);
		
static DEVICE_ATTR(channelBdata, S_IRUGO,
		wahoo_channel_data_show, NULL);
		
static DEVICE_ATTR(reset, S_IWUSR | S_IRUGO,
		wahoo_read_reset, wahoo_reset);

static struct device_attribute * wahoo_attrs[] = {
	&dev_attr_channelAstate,
	&dev_attr_channelBstate,
	&dev_attr_channelAdata,
	&dev_attr_channelBdata,
	&dev_attr_sampleRate,
	&dev_attr_trigger,
	&dev_attr_reset,
};

static unsigned char to_channel_id(struct device_attribute *attr)
{
	unsigned char ptr;

	for (ptr = 0; ptr < ARRAY_SIZE(wahoo_attrs); ptr++) {
		if (wahoo_attrs[ptr] == attr)
			return ptr%2;
	}

	return 0xFF;
}

static int __devinit wahoo_probe(struct spi_device *spi)
{
	struct wahoo_state *st;
	//struct wahoo_pdata *pdata;
	int ptr;
	int ret;

	/*pdata = spi->dev.platform_data;
	if (pdata == NULL) {
		dev_err(&spi->dev, "no device data specified\n");
		return -EINVAL;
	}*/

	st = kzalloc(sizeof(struct wahoo_state), GFP_KERNEL);
	if (st == NULL) {
		dev_err(&spi->dev, "no memory for device state\n");
		return -ENOMEM;
	}
	
	st->tx_buff = kzalloc(TX_BUFSIZE*sizeof(char), GFP_KERNEL);
	if (st->tx_buff == NULL) {
		dev_err(&spi->dev, "no memory for TX buffer\n");
		return -ENOMEM;
	}
	
	st->rx_buff = kzalloc(RX_BUFSIZE*sizeof(char), GFP_KERNEL);
	if (st->rx_buff == NULL) {
		dev_err(&spi->dev, "no memory for RX buffer\n");
		return -ENOMEM;
	}

	st->us = spi;
	
	//default values
	st->channel[0].id = 0;
	st->channel[0].on = 0;
	st->channel[0].gain = 1;
	st->channel[0].coupling = 0;
	st->channel[0].offset = 0;
	
	st->channel[1].id = 1;
	st->channel[1].on = 0;
	st->channel[1].gain = 1;
	st->channel[1].coupling = 0;
	st->channel[1].offset = 0;
	
	st->trigger.channel = 0;
	st->trigger.slope = 1;
	st->trigger.aut = 0;
	st->trigger.roll = 0;
	st->trigger.noise = 0;
	st->trigger.hf = 0;
	st->trigger.holdOff = 0;
	st->trigger.level = 0;
	

	mutex_init(&st->lock);


	for (ptr = 0; ptr < 7; ptr++) {
		ret = device_create_file(&spi->dev, wahoo_attrs[ptr]);
		if (ret) {
			dev_err(&spi->dev, "cannot create attribute %d\n",ptr);
			for (; ptr > 0; ptr--)
				device_remove_file(&spi->dev, wahoo_attrs[ptr]);

			return ret;
		}
	}
	
	spi_set_drvdata(spi, st);
	
	return 0;

}

static int __devexit wahoo_remove(struct spi_device *spi)
{
	struct wahoo_state *st = spi_get_drvdata(spi);
	int ptr;

	for (ptr = 0; ptr < 7; ptr++)
		device_remove_file(&spi->dev, wahoo_attrs[ptr]);

	kfree(st);
	return 0;
}

static struct spi_driver wahoo_driver = {
	.driver = {
		.name	= "wahoo",
		.owner	= THIS_MODULE,
	},
	.probe		= wahoo_probe,
	.remove		= __devexit_p(wahoo_remove),
};

static __init int wahoo_init(void)
{
	return spi_register_driver(&wahoo_driver);
}

static __exit void wahoo_exit(void)
{
	spi_unregister_driver(&wahoo_driver);
}

module_init(wahoo_init);
module_exit(wahoo_exit);

MODULE_AUTHOR("Pablo Carranza Velez <pablocarranza@gmail.com>");
MODULE_DESCRIPTION("Wahoo SPI driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("spi:wahoo");
