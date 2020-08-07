//
// Created by rjd67 on 2020/7/29.
//

#ifndef HIGAN_CODEC_H
#define HIGAN_CODEC_H

#include <cstring>
#include <string>
namespace higan
{
class Codec
{
public:
	Codec();
	~Codec();

	/**
	 * 将编码后的URL解码
	 * @param url
	 * @param len
	 * @return 解码后的URL
	 */
	static std::string DecodeUrl(const char* url, size_t len);

	/**
	 * 将编码后的URL解码
	 * @param url
	 * @param len
	 * @return 解码后的URL
	 */
	static std::string DecodeUrl(const std::string& url);

private:
	/**
	 * 十六进制字符串转十进制数字 "EC" = 236
	 * @param begin
	 * @param end
	 * @return -1 转换失败 转换成功返回对应值
	 */
	static int HexCharArrayToInt(const char* begin, const char* end);

	/**
	 * `A`-`F` `a`-`f` 转10-15 `0`-`9` 转 0-9
	 * @param character
	 * @return -1 转换失败 转换成功返回对应值
	 */
	static int HexCharToInt(char character);
};
}
#endif //HIGAN_CODEC_H
