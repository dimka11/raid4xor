#include <iostream>
#include <fstream>
#include <sstream>

//todo replace if's to std::map / tuples / etc.
//todo add padding with zeros and work with files with different lengths

struct FilesExists
{
	bool d1;
	bool d2;
	bool d3;
	bool d4;
};

char* xor_data(std::string d1, std::string d2, std::string d3, int& size)
{
	char const* d1_ = d1.c_str();
	char const* d2_ = d2.c_str();
	char const* d3_ = d3.c_str();

	auto len = strlen(d1_);

	//ASSUME ALL STRING EQUAL SIZE

	char* xor_ = new char[len];
	size = len;
	
	for (size_t i = 0; i <= len - 1; i++)
	{
		xor_[i] = d1_[i] ^ d2_[i] ^ d3_[i];
	}
	return xor_;
}

void createFiles()
{
	std::ofstream d1("d1");
	std::ofstream d2("d2");
	std::ofstream d3("d3");

	std::string test_data1 = "test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1 test_data1";
	std::string test_data2 = "test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2 test_data2";
	std::string test_data3 = "test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3 test_data3";
	d1 << test_data1;
	d2 << test_data2;
	d3 << test_data3;
	d1.close();
	d2.close();
	d3.close();

	int len_ = 0;
	char* xor_ = xor_data(test_data1, test_data2, test_data3, len_);

	std::ofstream d4("d4", std::ios::out | std::ios::binary | std::ios::app);
	d4.write(xor_, len_);
	d4.close();
}

void readFiles(const FilesExists* fe) {
	if ((!fe->d4) && fe->d1 && fe->d2 && fe->d3) {
		// recreate xor drive

		std::ifstream d1("d1", std::ios::in | std::ios::binary);
		std::ifstream d2("d2", std::ios::in | std::ios::binary);
		std::ifstream d3("d3", std::ios::in | std::ios::binary);
		char* b1 = new char[1024];
		char* b2 = new char[1024];
		char* b3 = new char[1024];
		d1.read(b1, 1024);
		int l1 = d1.gcount();
		d2.read(b2, 1024);
		int l2 = d2.gcount();
		d3.read(b3, 1024);
		int l3 = d3.gcount();

		int len = l1;

		char* xor_ = new char[len];

		for (size_t i = 0; i <= len - 1; i++)
		{
			xor_[i] = b1[i] ^ b2[i] ^ b3[i];
		}
		
		std::ofstream d4("d4", std::ios::out | std::ios::binary | std::ios::app);
		d4.write(xor_, len);
		d4.close();
	}
	if (((int)fe->d1 + (int) fe->d2 + (int) fe->d3) <= 1)
	{
		std::cout << "not enough files for recovery";
		return;
	}

	//restore

	std::ifstream d4("d4", std::ios::in | std::ios::binary);

	char* b1 = new char[1024];
	char* b2 = new char[1024];
	
	char* b4 = new char[1024];
	d4.read(b4, 1024);
	int l4 = d4.gcount();
	int len = l4;

	char* xor_ = new char[len];

	int buffer_used = 0;

	if (fe->d1)
	{
		std::ifstream d1("d1", std::ios::in | std::ios::binary);
		d1.read(b1, 1024);
		buffer_used = 1;
		int l1 = d1.gcount();
		d1.close();
	}

	if (fe->d2)
	{
		std::ifstream d1("d2", std::ios::in | std::ios::binary);
		if (buffer_used == 0)
		{
			d1.read(b1, 1024);
		}
		else {
			d1.read(b2, 1024);
		}
		
		int l2 = d1.gcount();
		d1.close();
	}

	if (fe->d3)
	{
		std::ifstream d1("d3", std::ios::in | std::ios::binary);
		d1.read(b2, 1024);
		int l3 = d1.gcount();
		d1.close();
	}

	for (size_t i = 0; i <= len - 1; i++)
	{
		xor_[i] = b1[i] ^ b2[i] ^ b4[i];
	}

	std::string drive_name;
	if (!fe->d1)
	{
		drive_name = "d1";
	}
	if (!fe->d2)
	{
		drive_name = "d2";
	}
	if (!fe->d3)
	{
		drive_name = "d3";
	}
	
	std::ofstream d0(drive_name, std::ios::out | std::ios::binary | std::ios::app);
	d0.write(xor_, len);
	d0.close();
}

FilesExists checkFilesExists()
{
	std::ifstream d1("d1");
	std::ifstream d2("d2");
	std::ifstream d3("d3");
	std::ifstream d4("d4");
	return { d1.good(), d2.good(), d3.good(), d4.good() };
}

int main(int argc, char* argv[])
{
	const auto fe_ = checkFilesExists();
	if (!(fe_.d1 || fe_.d2 || fe_.d3 || fe_.d4))
	{
		createFiles();
	}
	else if (fe_.d1 && fe_.d2 && fe_.d3 && fe_.d4)
	{
		std::cout << "all files is ok";
	}
	else {
		readFiles(&fe_);
	}
	return 0;
}
