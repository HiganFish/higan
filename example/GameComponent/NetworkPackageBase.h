//
// Created by rjd67 on 2020/10/13.
//
#ifdef UE4
#pragma once
#include "CoreMinimal.h"
#include "string"

#define uint32_t uint32
#define uint8_t uint8
#define ssize_t SSIZE_T
#undef UE4
#else
#include <cstdint>
#include <sys/types.h>
#include <cstring>
#include <string>

#ifndef HIGAN_NETWORKPACKAGEBASE_H
#define HIGAN_NETWORKPACKAGEBASE_H
#endif //HIGAN_NETWORKPACKAGEBASE_H
#endif

// 包类别
enum PackageCode : uint8_t
{
	HEART_BEAT,
	LOGIN,
	REGISTER
};


/*
数据包父类

提供 玩家Id 包类别 以及包ID 构建数据包头

派生出的子类 可以添加新的成员变量 并重写 SerializeToBuffer函数

重写SerializeToBuffer函数时

1. 可以调用 SerializeHeaderToBuffer 序列化包头部分
2. 然后编写子类中新的成员变量的序列化部分
3. 最后调用 SerializeCheckSumToBuffer生成校验和
*/
class NetworkPackageBase
{
public:
	// package_code 包类别  no 包序列号
	NetworkPackageBase(PackageCode package_code, uint32_t no);

	NetworkPackageBase();
	~NetworkPackageBase() = default;

public:

	/*
	序列化数据头 Body 校验和
	子类重写函数增加Body部分的序列
	序列化Body前调用SerializeHeaderToBuffer序列头部
	序列后调用SerializeCheckSumToBuffer序列尾部
	@parma BodySize Body部分长度
	@parma buffer 存放缓冲区的指针
	@parma buffer_len 缓冲区长度
	@return 成功 保存到缓冲区中的数据长度  失败返回-1
	*/
	virtual ssize_t SerializeToBuffer(uint8_t* buffer, size_t buffer_len);

protected:

	/*
	序列化 头部内容到缓冲区
	4 PackageBytes_  包总大小
	4 player_id_ 玩家ID
	1 PackageCode_ 包类别
	4 no_ 包ID
	4 CheckSum 在序列化头部的时候 初始化为0
	xxx Body
	4 CheckSum
	@parme body_size 数据部分字节数
	@parme buffer 存放序列化后数据的缓冲区
	@parme BufferSize 缓冲区长度
	@return 成功返回序列化的字节数 失败返回-1
	*/
	ssize_t SerializeHeaderToBuffer(size_t body_size, uint8_t* buffer, size_t buffer_len);

	/*
	计算校验和 并将其附加Header后
	@parma data 计算校验和的数据指针
	@parma data_len 计算校验和的数据长度
	*/
	void SerializeCheckSumToBuffer(uint8_t* data, size_t data_len);

public:

	/*
	从Buffer中反序列化数据
	@return 成功返回解析的数据长度 数据过少无法解析0 解析错误返回-1
	*/
	virtual ssize_t DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len);

protected:

	/*
	反序列化头部数据并保存
	@parma data 数据指针
	@parma data_len 数据长度
	@param body_len 保存body长度的指针 不需要则为空
	@return 解析成功返回解析的字节数  长度不够返回0 校验错误返回-1
	*/
	ssize_t DeSerializeHeaderFromBuffer(const uint8_t* data, size_t data_len, size_t* body_len);

public:

	uint32_t GetPackageLen() const;

protected:
	const static size_t BUFFER_LEN = 4096;

	// 不包括校验和
	const static size_t CHECKSUM_SIZE = 4;

	const static size_t HEADER_SIZE = 4 + 4 + 1 + 4 + CHECKSUM_SIZE;

	/*
	校验数据
	@param data 存放待校验数据的指针
	@param package_size 数据包的长度
	@return 正确返回true  否则返回false
	*/
	bool CheckData(const uint8_t* data, size_t package_size);
private:

	// 包序列化后的总字节数
	uint32_t package_len_;

	uint32_t player_id_;

	// 包的种类
	PackageCode package_code_;

	// 包的序号
	uint32_t no_;

	// 校验和 使用BCC算法 自身四字节不算入包长度 不参与校验  序列化时应放在Header后
	uint32_t check_sum_;

};
