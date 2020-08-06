//
// Created by rjd67 on 2020/8/5.
//

#ifndef HIGAN_ELECTRICITYBILL_H
#define HIGAN_ELECTRICITYBILL_H


#include <higan/EventLoop.h>
#include <string>
#include <unordered_set>

struct RoomInfo
{
	RoomInfo() = default;

	RoomInfo(const std::string& flat, const std::string& room):
			flatname(flat),
			roomname(room)
	{

	}

	std::string flatname;
	std::string roomname;

	bool operator==(const RoomInfo& room) const
	{
		return flatname + roomname == room.flatname + room.roomname;
	}

	std::string GetRoomInfoString() const
	{
		return flatname + roomname;
	}
};

class ElectricityBill
{
public:

	ElectricityBill(higan::EventLoop* loop, const std::string& text_root);
	~ElectricityBill();

	bool GetRoomFilePath(const std::string& request_url, std::string* path);

	void QueryBill(const higan::Timer& timer);

private:
	higan::EventLoop* loop_;
	std::string text_root_;

	higan::File room_file_;

	const static std::string ROOM_MAP_FILE;
	const static std::string SH_FILE;
	std::vector<RoomInfo> room_informations_;

	bool GetInfoFromUrl(const std::string& request_url, RoomInfo* room);

	std::string DoQuery(const RoomInfo& room_info);

	void AddNewRoom(const RoomInfo& room_info);
};


#endif //HIGAN_ELECTRICITYBILL_H
