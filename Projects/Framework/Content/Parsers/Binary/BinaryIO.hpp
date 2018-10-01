#pragma once
#include "FrameworkPCH.h"

//ErrorStates
#define B_READER_EOF	0x00000001

//_BASE::BinaryIO
class BinaryIO {
public:
	BinaryIO() :m_HeadOffSet{}, m_Flags{}
	{
	};

protected:
	void MoveBufferHead(unsigned int offSet)
	{
		m_HeadOffSet += offSet;
	};
	unsigned int m_HeadOffSet;
	unsigned char m_Flags;
};

//_DERIVED::BinaryReader
class BinaryReader : public BinaryIO
{
public:
	BinaryReader() :BinaryIO(), m_FileIn{}, m_LengthInBytes{}
	{
	};

	bool EndOfFile() { return static_cast<bool>(m_Flags & B_READER_EOF); };

	void Open(std::string& fileName)
	{
		if (!m_FileIn.is_open())
		{
			m_HeadOffSet = 0;
			m_FileIn.open(fileName, std::ios::out | std::ios::binary | std::ios::ate);
			m_LengthInBytes = size_t(m_FileIn.tellg());
		}
	};

	void Close()
	{
		if (m_FileIn.is_open())
		{
			m_FileIn.close();
			m_HeadOffSet = 0;
		}
	};

	//--Read Atom
	template<typename T>
	void Read(T& toRead)
	{
		if (m_FileIn.is_open())
		{
			int byteSize{ sizeof(toRead) };
			if ((m_HeadOffSet + byteSize) <= m_LengthInBytes)
			{
				m_FileIn.seekg(m_HeadOffSet);
				m_FileIn.read((char *)&toRead, byteSize);
				MoveBufferHead(byteSize);
			}
			else
			{
				m_Flags |= B_READER_EOF;
			}
		}
	};
	//-------------------*/

	//Read Molecules
	void Read(std::string& toRead)
	{
		//Read the strings length
		unsigned char strLength{};
		Read(strLength);

		//Read the chars
		for (size_t i = 0; i < size_t(strLength); i++)
		{
			char temp{};
			Read(temp);
			toRead.push_back(temp);
		}
	};

	template<typename T>
	void Read(std::vector<T>& toRead)
	{
		unsigned char vectorSize{};
		Read(vectorSize);

		for (unsigned char i = 0; i < vectorSize; i++)
		{
			T tempVal{};
			Read(tempVal);

			toRead.push_back(tempVal);
		}
	};
	//-------------------*/

private:
	std::ifstream m_FileIn;
	size_t m_LengthInBytes;
};

//_DERIVED::BinaryWriter
class BinaryWriter : public BinaryIO
{
public:
	BinaryWriter():BinaryIO(), m_FileOut{}
	{
	};

	void Open(std::string& fileName, bool shouldClear = 0)
	{
		if (!m_FileOut.is_open())
		{
			m_HeadOffSet = 0;
			int flags = std::ios::out | std::ios::binary;
			shouldClear ? flags |= std::ios::trunc : 0;
			m_FileOut.open(fileName, flags);
		}
	};

	void Close()
	{
		if (m_FileOut.is_open()) m_FileOut.close();
	};

	template<typename T>
	void Write(T&& toWrite)
	{
		T temp{ toWrite };
		Write(temp);
	}

	template<typename T>
	void Write(T& toWrite)
	{
		if (m_FileOut.is_open())
		{
			int byteSize{ sizeof(toWrite) };
			m_FileOut.seekp(m_HeadOffSet);
			m_FileOut.write((const char*)&toWrite, byteSize);
			MoveBufferHead(byteSize);
		}
	};
	void Write(std::string& toWrite)
	{
		//Write away the strings length
		unsigned char strLength{ unsigned char(toWrite.length()) };
		Write(strLength);

		//Write away the actual chars
		for (char& c : toWrite)
		{
			Write(c);
		}
	};

	template<typename T>
	void Write(std::vector<T>& toWrite)
	{
		unsigned char vectorSize{ unsigned char(toWrite.size()) };
		Write(vectorSize);

		for (T& var : toWrite)
		{
			Write(var);
		}
	}
private:
	std::ofstream m_FileOut;
};