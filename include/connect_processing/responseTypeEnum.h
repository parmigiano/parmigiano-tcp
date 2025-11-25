#pragma once

enum responseType {
	send_message,
	read_message,
	edit_message,
	pin_message,
	delete_message,
	user_typing,
	user_online_status,
	get_unread_message_count,
	RESPONSE_TYPE_COUNT
};