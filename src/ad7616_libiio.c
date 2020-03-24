#include "iio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * 功 能: 通道属性检索
 * 参 数: chn 	—— AD 通道
 * 		  attr 	—— 需要检索的属性
 * 返 回: true  —— AD通道存在属性attr
 * 		  false —— AD通道不存在属性attr
 */
static bool channel_has_attr(struct iio_channel *chn, const char *attr)
{
	unsigned int i, nb = iio_channel_get_attrs_count(chn);
	// printf("channel_num:%d\n",nb);
	for (i = 0; i < nb; i++)
		// printf("channel attr = %s\n", iio_channel_get_attr(chn, i));
		if (!strcmp(attr, iio_channel_get_attr(chn, i)))
			return true;
	return false;
}
/*
 * 功 能: 获取通道chan的采样值
 * 参 数: chn —— AD 通道
 * 返 回: chn 通道的采样值
 */
static double get_channel_value(struct iio_channel *chn)
{
	char buf[1024];
	double val;

	if (channel_has_attr(chn, "processed"))
	{
		iio_channel_attr_read(chn, "processed", buf, sizeof(buf));
		val = strtod(buf, NULL);
	}
	else
	{
		iio_channel_attr_read(chn, "raw", buf, sizeof(buf));
		val = strtod(buf, NULL);

		if (channel_has_attr(chn, "offset"))
		{
			iio_channel_attr_read(chn, "offset", buf, sizeof(buf));
			val += strtod(buf, NULL);
		}

		if (channel_has_attr(chn, "scale"))
		{
			iio_channel_attr_read(chn, "scale", buf, sizeof(buf));
			val *= strtod(buf, NULL);
		}
	}

	val = val * 0.305176;
	return val;
}

int main(void)
{
	struct iio_context *ctx;
	unsigned int i, nb_devices, n;
	unsigned int nb_channels;
	double channel_value;
	struct iio_device *dev;
	struct iio_channel *chn;

	// create context
	ctx = iio_create_local_context();
	if (!ctx)
	{
		printf("create context failed\n");
		exit(0);
	}

	// get device count
	nb_devices = iio_context_get_devices_count(ctx);
	printf("number of devices:%d\n", nb_devices);

	// get iio device 1
	printf("open iio device1!\n");
	dev = iio_context_get_device(ctx, 1);

	nb_channels = iio_device_get_channels_count(dev);
	printf("number of channels:%d\n", nb_channels);

	

	printf("channel_value:\n");

	for (n = 0; n < 100; n++)
	{
		for (i = 0; i < nb_channels; i++)
		{
			chn = iio_device_get_channel(dev, i);
			channel_value = get_channel_value(chn);
			printf("channel[%d]:%.2fmV\n", i, channel_value);
		}
		sleep(3);
		printf("-------------------------\n");
	}

	iio_context_destroy(ctx);

	return 0;
}
