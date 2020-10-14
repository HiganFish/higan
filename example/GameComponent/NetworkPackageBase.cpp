//
// Created by rjd67 on 2020/10/13.
//
#include "NetworkPackageBase.h"

NetworkPackageBase::NetworkPackageBase(PackageCode package_code, uint32_t no):
package_len_(0),
player_id_(0x01234567),
package_code_(package_code),
no_(no),
check_sum_(0)
{

}

NetworkPackageBase::NetworkPackageBase():
package_len_(0),
player_id_(0),
package_code_(static_cast<PackageCode>(0)),
no_(0),
check_sum_(0)
{
}


ssize_t NetworkPackageBase::SerializeToBuffer(uint8_t* buffer, size_t buffer_len)
{
	ssize_t data_len = SerializeHeaderToBuffer(0, buffer, buffer_len);
	if (data_len == -1)
	{
		return -1;
	}

	SerializeCheckSumToBuffer(buffer, data_len);

	return data_len;
}

ssize_t NetworkPackageBase::DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len)
{
	return DeSerializeHeaderFromBuffer(buffer, buffer_len, nullptr);
}

ssize_t NetworkPackageBase::SerializeHeaderToBuffer(size_t body_size, uint8_t* buffer, size_t buffer_len)
{
	package_len_ = HEADER_SIZE + body_size;

	if (buffer_len < package_len_)
	{
		return 0;
	}

	ssize_t result_bytes = 0;

	memcpy(buffer + result_bytes, &package_len_, sizeof package_len_);
	result_bytes += sizeof package_len_;


	memcpy(buffer + result_bytes, &player_id_, sizeof player_id_);
	result_bytes += sizeof player_id_;

	memcpy(buffer + result_bytes, &package_code_, sizeof package_code_);
	result_bytes += sizeof package_code_;

	memcpy(buffer + result_bytes, &no_, sizeof no_);
	result_bytes += sizeof no_;

	/*
	 *
	 * 2020年10月15日01:35:56
	 *
	 * 记录一次 自己头被锤爆 起初是没有下面的一行memcpy check_sum_初始为0
	 *
	 * 开始只是简单的增加四个字节 留出位置  (我是用的这种校验方式 应该是将这四个字节 清零!)
	 *
	 * 然而内存中这四个字节会残留之前内容 (BUG发生的时候 残留的正好是上一次的check_sum_)
	 * 然而计算新的check_sum_的时候 这四个字节的位置也是参与计算的 导致了新的check_sum_虽然是正确的
	 * 但是计算时使用了之前这四个字节的残留数据 之后这四个字节数据就被覆盖成了新的check_sum_
	 *
	 * 导致了最终数据流 再次BBC校验不为0  正好需要在 ^ 一次 被覆盖的原check_sum_
	 * */
	memcpy(buffer + result_bytes, &check_sum_, CHECKSUM_SIZE);
	result_bytes += CHECKSUM_SIZE;

	if (result_bytes != HEADER_SIZE)
	{
		return -1;
	}

	return result_bytes;
}

ssize_t NetworkPackageBase::DeSerializeHeaderFromBuffer(const uint8_t* data, size_t data_len, size_t* body_len)
{
	// 长度小于最小长度
	if (data_len < HEADER_SIZE)
	{
		return 0;
	}

	uint32_t package_len = 0;

	memcpy(&package_len, data, sizeof package_len);


	// 过滤大包
	if (package_len > BUFFER_LEN)
	{
		return -1;
	}

	// 缓冲区数据长度 小于数据包长度
	if (package_len > data_len)
	{
		return 0;
	}

	if (!CheckData(data, package_len))
	{
		return -1;
	}

	size_t data_sub = sizeof package_len;

	memcpy(&player_id_, data + data_sub, sizeof player_id_);
	data_sub += sizeof player_id_;

	memcpy(&package_code_, data + data_sub, sizeof package_code_);
	data_sub += sizeof package_code_;

	memcpy(&no_, data + data_sub, sizeof no_);
	data_sub += sizeof no_;

	data_sub += CHECKSUM_SIZE;

	if (data_sub != HEADER_SIZE)
	{
		return -1;
	}

	if (body_len)
	{
		*body_len = package_len - data_sub;
	}
	return data_sub;
}

bool NetworkPackageBase::CheckData(const uint8_t* data, size_t package_size)
{
	uint32_t result = 0;
	for (uint32_t i = 0; i < package_size; ++i)
	{
		result = result ^ data[i];
	}

	return result == 0;
}

void NetworkPackageBase::SerializeCheckSumToBuffer(uint8_t* data, size_t data_len)
{
	check_sum_ = 0;
	for (size_t i = 0; i < data_len; ++i)
	{
		check_sum_ = check_sum_ ^ data[i];
	}

	memcpy(data + HEADER_SIZE - CHECKSUM_SIZE, &check_sum_, CHECKSUM_SIZE);
}

uint32_t NetworkPackageBase::GetPackageLen() const
{
	return package_len_;
}
