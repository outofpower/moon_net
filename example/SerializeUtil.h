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
		static  message serialize(EMsgID msgid, const TMsg& t)
		{
			uint16_t size = t.ByteSize();
			message msg(sizeof(uint16_t)+size);
			binary_writer bw(msg);
			bw << uint16_t(msgid);

			uint8_t tmp[10000];
			assert(size < sizeof(tmp));
			t.SerializeToArray(tmp, size);
			bw.write(tmp, size);
			return std::move(msg);
		}

		static  message serialize(EMsgID msgid)
		{
			message msg(sizeof(uint16_t));
			binary_writer bw(msg);
			bw << uint16_t(msgid);
			return std::move(msg);
		}
	};
}
