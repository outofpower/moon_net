#include "stdafx.h"
#include "CppUnitTest.h"
#include <common/memory_stream.hpp>
#include <common/binary_reader.hpp>
#include <common/binary_writer.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace moon;
namespace UnitTest
{		
	TEST_CLASS(MemoryStreamTest)
	{
	public:
		TEST_METHOD(TestBasicWriteRead)
		{
			memory_stream ms;

			int i = 10123;
			float f = 0.232356f;
			bool b = false;
			char* str = "adadad112445";

			ms.write_back(&i, 0, 1);
			ms.write_back(&f, 0, 1);
			ms.write_back(&b, 0, 1);
			ms.write_back(str, 0, strlen(str) + 1);

			int oi = 0;
			float of = 0.0f;
			bool ob = false;
			char ostr[1024] = "0";

			ms.read(&oi, 0, 1);
			ms.read(&of, 0, 1);
			ms.read(&ob, 0, 1);
			ms.read(ostr, 0, 100);
			Assert::AreEqual(i, oi);
			Assert::AreEqual(f, of);
			Assert::AreEqual(b, ob);
			Assert::AreEqual(str, ostr);
		}

		TEST_METHOD(TestOffsetWriteRead)
		{
			memory_stream ms;
			int a[] = { 10,23,14,12,344,78,112,89 };
			ms.write_back(a, 4, 4);//write: 344,78,112, 89
			int oa[10];
			ms.read(oa, 4, 4);

			Assert::AreEqual(a[4], oa[4]);
			Assert::AreEqual(a[5], oa[5]);
			Assert::AreEqual(a[6], oa[6]);
			Assert::AreEqual(a[7], oa[7]);
		}

		TEST_METHOD(TestSeek)
		{
			memory_stream ms;
			int a[] = { 10,23,14,12,344,78,112,89 };
			ms.write_back(a, 0, 8);
			ms.seek(sizeof(int) * 3, memory_stream::Current);

			int o1 = 0;
			ms.read(&o1, 0, 1);
			Assert::AreEqual(12, o1);
			
			ms.seek(-static_cast<int>(sizeof(int)) * 5, memory_stream::End);
			ms.read(&o1, 0, 1);
			Assert::AreEqual(12, o1);

			ms.seek(static_cast<int>(sizeof(int)) *1, memory_stream::Begin);
			ms.read(&o1, 0, 1);
			Assert::AreEqual(23, o1);
		}

		TEST_METHOD(TestWriteFront)
		{
			memory_stream ms;

			memory_stream ms2(64, 8);
			int n = 109;
			ms2.write_front(&n, 0, 1);
			ms.write_front(&n, 0, 1);

			int on = 0;
			ms2.read(&on, 0, 1);
			Assert::AreEqual(n, on);

			int on2 = 0;
			ms.read(&on2, 0, 1);
			Assert::AreEqual(0, on2);
		}

		TEST_METHOD(TestMemoryCopy)
		{
			memory_stream ms;

			int n = 109;
			ms.write_back(&n, 0, 1);

			for(int i = 0;  i < 1000; i++)
			{
				ms.write_back(&i, 0, 1);
			}

			int on = 0;
			ms.read(&on, 0, 1);

			Assert::AreEqual(109, on);
			ms.read(&on, 0, 1);
			Assert::AreEqual(0,on);
			ms.seek(sizeof(int) * 998, memory_stream::Current);
			ms.read(&on, 0, 1);
			Assert::AreEqual(999, on);


		}

		TEST_METHOD(TestReaderWriter)
		{
			moon::memory_stream ms(4);
			int n = 109;
			float f = 19.9801f;
			bool b = true;
			double d = 892.7881;
			std::string str = "12355";

			binary_writer bw(ms);
			bw.write(n);
			bw.write(f);
			bw.write(b);
			bw.write(d);
			bw.write(str);

			std::vector<int> vec = { 1,2,3,4,5 };
			bw.write_vector(vec);

			binary_reader br(ms.data(), ms.size());

			auto a1 = br.read<int>();
			auto a2 = br.read<float>();
			auto a12 = br.read < bool >();
			auto a3 = br.read<double>();
			auto a4 = br.read<std::string>();

			auto v = br.read_vector<int>();

			Assert::AreEqual(n, a1);
			Assert::AreEqual(f, a2);
			Assert::AreEqual(b, a12);
			Assert::AreEqual(d, a3);
			Assert::AreEqual(str, a4);
			Assert::AreEqual(vec.size(), v.size());
		}

	};
}