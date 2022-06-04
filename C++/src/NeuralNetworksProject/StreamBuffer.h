#pragma once
#include <vector>
#include <fstream>

/*
 *Hemos creado este namespace con la finalidad
 *de manejar un conjunto de datos empaquetados
 *sin importar el tipo.
 *
 *La idea era emular una funcionalidad similar a los
 *packet de SFML para poder enviar informacion
 *entre eventos.
 *
 *Adicionalmente hemos implementado la escritura de
 *ficheros binarios.
 * 
 */

namespace stream_buff
{
	typedef char* data;
	typedef char** buffer;

	template <class T>
	T get(const stream_buff::buffer & buff, int index)
	{
		return (*reinterpret_cast<T*>(buff[index]));
	}

	template <class T>
	void insert(stream_buff::buffer & buff, T* value, int index)
	{
		buff[index] = reinterpret_cast<data>(value);
	}

	template <class T>
	void save(std::vector<T> & o, const std::string & fileName, short fstream_mask = 0)
	{
		std::ofstream fs(fileName, std::fstream::binary | fstream_mask);

		int len = o.size();
		fs.write(reinterpret_cast<data>(&len), sizeof(int));
		
		T * arr = o.data();

		fs.write(reinterpret_cast<data>(arr), sizeof(T) * len);
		fs.close();

	}

	template <class T>
	void recover(std::vector<T>& o, const std::string & fileName, short fstream_mask = 0)
	{
		std::ifstream myFile(fileName, std::ios::binary | fstream_mask);

		int len;
		myFile.read(reinterpret_cast<data>(&len), sizeof(int));

		T* arr = new T[len*sizeof(T)];

		myFile.read(reinterpret_cast<data>(arr), sizeof(T)* len);
		myFile.close();

		o = std::vector<T>(arr, arr + len);

		delete[]arr;

	}


	template <class T>
	void save(std::vector<T> & o, int num_elements, const std::string & fileName, bool save_quantity = false, short fstream_mask = 0)
	{
		std::ofstream fs(fileName, std::fstream::binary | fstream_mask);

		if (save_quantity)
			fs.write(reinterpret_cast<data>(&num_elements), sizeof(int));

		T* arr = o.data();

		fs.write(reinterpret_cast<data>(arr), sizeof(T) * num_elements);
		fs.close();

	}

	template <class T>
	void recover(std::vector<T>& o, int num_elements, const std::string & fileName, short fstream_mask = 0)
	{

		std::ifstream myFile(fileName, std::ios::binary | fstream_mask);

		T* arr = new T[num_elements];

		myFile.read(reinterpret_cast<data>(arr), sizeof(T)* num_elements);
		myFile.close();

		o.assign(arr, arr + num_elements);
		

	}


	template <class T>
	void save(const T & o, const std::string & fileName, short fstream_mask = 0)
	{
		std::ofstream fs(fileName, std::fstream::binary | fstream_mask);

		fs.write(reinterpret_cast<data>(&o), sizeof(T));

		fs.close();

	}

	template <class T>
	void recover(T & o, const std::string & fileName, short fstream_mask = 0)
	{
		std::ifstream myFile(fileName, std::ios::binary | fstream_mask);
		
		myFile.read(reinterpret_cast<data>(&o), sizeof(T));

		myFile.close();
	}
	
	
}