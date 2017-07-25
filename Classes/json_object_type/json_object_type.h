/*==============================================================*\
                
                a sort of JSON lib by Lx
                email: imbosslx@sina.com
                last_update: 2016.12.18 


\*==============================================================*/
#ifndef _DEU_JSON_OBJECT_TYPE_H_
#define _DEU_JSON_OBJECT_TYPE_H_



#include <string>
#include <vector>
#include <fstream>


namespace tudo_push_box{


	class json_object_type{
	public:
		std::string key, text;
		std::vector<json_object_type> children;

		std::string get_key();
		void set_key(std::string _k);

		std::string get_text();
		void set_text(std::string _t);

		int get_type();//1 = string ,2 = value,3 = object,4 = array 

		std::string get_string();
		int get_int();
		long long get_long_long();
		double get_double();
		float get_float();

		void put_string(std::string v);
		void put_int(int v);
		void put_long_long(long long v);
		void put_double(double v);
		void put_float(float v);

		json_object_type &at(std::string _key);
		json_object_type &at(size_t _i);
		json_object_type add_object(json_object_type jo);
		json_object_type add_object(std::string _k,std::string _v);
		json_object_type add_object(std::string _k,long long _v);
		json_object_type add_object(std::string _k,double _v);

		json_object_type clone();


		json_object_type();
		json_object_type(std::string s);
		json_object_type(std::string s,std::string _k);

		json_object_type & operator[](std::string _k);
		json_object_type & operator[](size_t _i);


		static json_object_type phraze(std::string);
		static std::string json_object_type_to_string(const json_object_type &,int d = -1);
		static std::string read_file_all(std::fstream&);

	};

}





#endif