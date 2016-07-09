#pragma once
#include <cstdint>
#include <memory>
#include <message.h>
#include "MessageID.h"
#include "UserContext.h"
#include "user_id.h"

namespace moon
{
	class SerializeUtil
	{
	public:

		template<class TMsg>
		static  buffer_ptr serialize(EMsgID msgid, const TMsg& t)
		{
			uint16_t size = t.ByteSize();
			auto buff = buffer::create(sizeof(uint16_t) + size);
			(*buff) << (uint16_t(msgid));
			uint8_t tmp[10000];
			assert(size < sizeof(tmp));
			t.SerializeToArray(tmp, size);
			buff->append(tmp, size);
			return buff;
		}

		static  buffer_ptr serialize(EMsgID msgid)
		{
			auto buff = buffer::create(sizeof(uint16_t));
			(*buff) << (uint16_t(msgid));
			return buff;
		}
	};
}
