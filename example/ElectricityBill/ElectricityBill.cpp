//
// Created by rjd67 on 2020/8/5.
//

#include <fcntl.h>
#include <map>
#include <higan/utils/File.h>
#include <higan/utils/Logger.h>
#include <higan/utils/System.h>

#include "ElectricityBill.h"

const std::string ElectricityBill::ROOM_MAP_FILE = "room.txt";
const std::string ElectricityBill::SH_FILE = "dianfei.sh";

ElectricityBill::ElectricityBill(higan::EventLoop* loop, const std::string& text_root):
		loop_(loop),
		text_root_(text_root),
		room_file_(text_root_ + ROOM_MAP_FILE),
		room_informations_()
{
	higan::FileForRead file_read(text_root + ROOM_MAP_FILE);

	std::string flatname;
	std::string roomname;
	while (!(flatname = file_read.ReadLine()).empty())
	{
		roomname = file_read.ReadLine();

		RoomInfo room_info(flatname, roomname);
		room_informations_.push_back(room_info);

		LOG_INFO << higan::Fmt("read room: %s from file", room_info.GetRoomInfoString().c_str());
	}

	higan::Timer timer{"QueryBill", 60 * 1000, true,
					std::bind(&ElectricityBill::QueryBill, this, std::placeholders::_1)};
	loop_->AddTimer(timer);

}

ElectricityBill::~ElectricityBill()
{

}

std::map<std::string, std::string> name_map{
		{"M", "梅"}, {"Z", "竹"}, {"X", "杏"}, {"S", "松"},
		{"T", "桃"}};

bool ElectricityBill::GetRoomFilePath(const std::string& connname, const std::string& request_url, std::string* path)
{
	RoomInfo room_info;

	if (!GetInfoFromUrl(request_url, &room_info))
	{
		LOG_WARN << "conn: " << connname << " bad url: " << request_url;
		return false;
	}
	else
	{
		LOG_INFO << "conn: " << connname <<  " get room: " << room_info.GetRoomInfoString();
	}

	auto timer_result = std::find(room_informations_.begin(), room_informations_.end(), room_info);
	if (timer_result == room_informations_.end())
	{
		AddNewRoom(room_info);
		LOG_INFO << "conn: " << connname << " add new room: " << room_info.GetRoomInfoString();
	}

	DoQuery(room_info, "");

	*path = text_root_ + "df-" + room_info.GetRoomInfoString() + ".txt";

	return true;
}

bool ElectricityBill::GetInfoFromUrl(const std::string& request_url, RoomInfo* room_info)
{
	// "/M2A4000"
	if (request_url.length() != 8 && request_url.length() != 7)
	{
		return false;
	}

	room_info->flatname += request_url[1];

	auto find_result = name_map.find(room_info->flatname);
	if (find_result != name_map.end())
	{
		room_info->flatname = find_result->second;
	}
	else
	{
		return false;
	}


	if (request_url[2] < '0' || request_url[2] > '9')
	{
		return false;
	}
	room_info->flatname += request_url[2];
	room_info->flatname += "楼"; // 梅2楼

	if (request_url[3] < 'A' || request_url[3] > 'C')
	{
		return false;
	}

	room_info->roomname = request_url[3];

	//  /M2A4000

	for (int i = 4; i < std::min(8, static_cast<int>(request_url.length())); ++i)
	{
		if (request_url[i] < '0' || request_url[i] > '9')
		{
			return false;
		}
		else
		{
			room_info->roomname += request_url[i];
		}
	}

	return true;
}

void ElectricityBill::QueryBill(const higan::Timer& timer)
{
	LOG_INFO << "----------Query Bill Start------------";

	for (const auto & room_info : room_informations_)
	{
		LOG_INFO << "Query Result " <<
					DoQuery(room_info, "auto").c_str();
	}

	LOG_INFO << "----------Query Bill End------------";
}

std::string ElectricityBill::DoQuery(const RoomInfo& room_info, const std::string& exoutput)
{
	return higan::System::RunShellCommand("sh",
			{text_root_ + SH_FILE, room_info.flatname, room_info.roomname, exoutput});
}

void ElectricityBill::AddNewRoom(const RoomInfo& room_info)
{
	room_informations_.push_back(room_info);

	room_file_.Append(room_info.flatname + "\n");
	room_file_.Append(room_info.roomname + "\n");
	room_file_.Flush();
}
