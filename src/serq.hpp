//=====================================================================================================================================
//	The MIT License (MIT)
//	Copyright (c) 2015 Austin Salgat
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
//	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
//	merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished
//	to do so, subject to the following conditions:
//	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
//	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
//	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================================

#pragma once
#ifndef SERIALIZE_QUEUE_H
#define SERIALIZE_QUEUE_H

#include <vector>
#include <stack>
#include <memory>
#include <fstream>
#include <inttypes.h>
#include <cstring>
#include <string>
#include <iomanip>
#include <utility>
#include <map>
#include <queue>
#include <stack>
#include <tuple>

namespace serq {
	/**
	 * Returns a reversed tuple. (thanks to http://stackoverflow.com/questions/25119048/reversing-a-c-tuple)
	 */
	template<typename T, typename TT = typename std::remove_reference<T>::type, size_t... I>
	auto reverse_impl(T&& t, std::index_sequence<I...>) -> std::tuple<typename std::tuple_element<sizeof...(I) - 1 - I, TT>::type...> {
		return std::make_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<T>(t))...);
	}

	template<typename T, typename TT = typename std::remove_reference<T>::type>
	auto reverse(T&& t) -> decltype(reverse_impl(std::forward<T>(t), std::make_index_sequence<std::tuple_size<TT>::value>())) {
		return reverse_impl(std::forward<T>(t), std::make_index_sequence<std::tuple_size<TT>::value>());
	}

	template <class T>
	struct tag {};

	/**
	 * FIFO queue that is able to convert back and forth from and to binary (serialized) data.
	 */
	class SerializeQueue {
	private:
		std::vector<uint64_t> variable_lengths; // Used to store the lengths of variable sized objects
		std::stack<std::vector<unsigned char>> binary_data;
		
		std::vector<unsigned char> original_serialized_blob;
		std::vector<unsigned char> serialized_blob;
		
		// Helper methods
		/**
		 * Adds the char blob to the serialized_blob.
		 */
		void PushBlob(std::vector<unsigned char> const& char_blob) {
			for (unsigned char const character : char_blob) {
				serialized_blob.push_back(character);
			}
		}
		
		void PushBlob(std::vector<unsigned char> const& char_blob, int offset) {
			//for (unsigned char const character : char_blob) {
			for (int index = offset; index < char_blob.size(); ++index) {
				serialized_blob.push_back(char_blob[index]);
			}
		}
		
		/**
		 * Pushes data directly to serialized_data.
		 */
		void PushToSerializedBlob(uint64_t const data) {
			for (std::size_t index = 0; index < sizeof(data); ++index) {
				serialized_blob.push_back((data >> (index*8)) & 0xFF);
			}
		}
		
		/**
		 * Returns uint64_t value at provided serialized_blob offset.
		 */
		uint64_t ReadSerializedBlob(int const offset) const {
			uint64_t data = 0x00;
			for (int index = sizeof(data)-1; index >= 0; --index) {
				unsigned char character = serialized_blob[index+offset];
				data |= static_cast<uint64_t>(character) << static_cast<uint64_t>(index*8);
			}
			
			return *reinterpret_cast<uint64_t*>(&data);
		}

		uint64_t ReadSerializedBlob(int const offset, std::vector<unsigned char> const& data_vector) const {
			uint64_t data = 0x00;
			for (int index = sizeof(data)-1; index >= 0; --index) {
				unsigned char character = data_vector[index+offset];
				data |= static_cast<uint64_t>(character) << static_cast<uint64_t>(index*8);
			}
			
			return *reinterpret_cast<uint64_t*>(&data);
		}			
		
		void WriteToOffset(uint64_t const data, int const offset) {
			for (int index = 0; index < sizeof(data); ++index) {
				serialized_blob[index+offset] = (data >> (index*8)) & 0xFF;
			}
		}
	
		/**
		 * Returns length of current variable length object (that is not yet popped).
		 */
		uint64_t GetVariableLength() const {
			auto offset_counter = ReadSerializedBlob(0);
			return ReadSerializedBlob((offset_counter)*8);
		}
		
		/**
		 * Decrements offset counter for variable length variables.
		 */
		void DecrementVariableLengthCounter() {
			auto offset_counter = ReadSerializedBlob(0);
			--offset_counter;
			WriteToOffset(offset_counter, 0x00);
		}
		
		/**
		 * Template specialized overloads (tag section)
		 */
		 
		// Push Section -----------------------------------------------------------
		
		void push(tag<uint64_t>, uint64_t const data) {
			push(data);
		}
		
		void push(tag<unsigned int>, unsigned int const data) {
			push(data);
		}
		
		void push(tag<int>, int const data) {
			push(data);
		} 
		 
		void push(tag<unsigned char>, unsigned char const data) {
			push(data);
		}
		
		void push(tag<float>, float const data) {
			push(data);
		}
		
		void push(tag<double>, double const data) {
			push(data);
		}
		
		void push(tag<std::string>, std::string const& data) {
			push(data);
		}
		
		// STL Containers
		
		template<class T1, class T2>
		void push(tag<std::pair<T1, T2>>, std::pair<T1, T2> const& data) {
			push(data);
		}
		
		template<class... Values, std::size_t... IndexSequence>
		void push_tuple(std::tuple<Values...> const& data, std::index_sequence<IndexSequence...>) {
			// Due to how the tuple is stored, it needs to be reversed (pop order is backwards).
			// Todo: A more efficient way to push or pop in reverse
			auto new_tuple = reverse(data);
			(int[]){ 0, (push(std::get<IndexSequence>(new_tuple)), 0)... };
		}
		
		template<class... Values>
		void push(tag<std::tuple<Values...>>, std::tuple<Values...> const& data) {
			
			push_tuple(data, std::make_index_sequence<sizeof...(Values)>());
		}
		
		template<class T>
		void push(tag<std::vector<T>>, std::vector<T> const& data_vector) {
			push_vector(data_vector);
		}
		
		template<class T1, class T2>
		void push(tag<std::map<T1, T2>>, std::map<T1, T2> const& data_map) {
			push_map(data_map);
		}
		
		template<class T>
		void push(tag<std::queue<T>>, std::queue<T> data_queue) {
			push_queue(data_queue);
		}
		
		template<class T>
		void push(tag<std::stack<T>>, std::stack<T> data_stack) {
			push_stack(data_stack);
		}
		
		// Pop Section -----------------------------------------------------------
		
		template<class T>
		T pop_generic() {
			uint64_t data = 0x00;
			for (int index = sizeof(data)-1; index >= 0; --index) {
				unsigned char character = serialized_blob.back();
				serialized_blob.pop_back();
				data |= static_cast<uint64_t>(character) << static_cast<uint64_t>(index*8);
			}
			
			return *reinterpret_cast<T*>(&data);
		}
		
		uint64_t pop(tag<uint64_t>) {
			return pop_generic<uint64_t>();
		}
		
		unsigned int pop(tag<unsigned int>) {
			return pop_generic<unsigned int>();
		}
		
		int pop(tag<int>) {
			return pop_generic<int>();
		}
		
		unsigned char pop(tag<unsigned char>) {
			return pop_generic<unsigned char>();
		}
		
		float pop(tag<float>) {
			return pop_generic<float>();
		}
		
		double pop(tag<double>) {
			return pop_generic<double>();
		}
		
		// STL Containers
		
		template<class T1, class T2>
		std::pair<T1, T2> pop(tag<std::pair<T1, T2>>) {
			return pair_pop<T1, T2>();
		}
		
		template<class... Values>
		auto pop(tag<std::tuple<Values...>>) {
			return std::make_tuple<Values...>(std::move(pop<Values>())...);
		}
		
		template<class T>
		std::vector<T> pop(tag<std::vector<T>>) {
			return vector_pop<T>();
		}
		
		template<class T1, class T2>
		std::map<T1, T2> pop(tag<std::map<T1, T2>>) {
			return map_pop<T1, T2>();
		}
		
		template<class T>
		std::queue<T> pop(tag<std::queue<T>>) {
			return queue_pop<T>();
		}
		
		template<class T>
		std::stack<T> pop(tag<std::stack<T>>) {
			return stack_pop<T>();
		}
		
	public:
		SerializeQueue() {}
		
		/**
		 * Clears queue completely.
		 */
		void Clear() {
			variable_lengths.clear(); // Used to store the lengths of variable sized objects
			binary_data = std::stack<std::vector<unsigned char>>();
			
			original_serialized_blob.clear();
			serialized_blob.clear();
		}
		
		/**
		 * Returns a vector of char which represents the binary state of the serialized data.
		 * Data is stored in little-endian order (least significant byte in the smallest address).
		 *
		 * Note: vector is used because it automatically stores the size of the char blob.
		 */
		std::vector<unsigned char> Serialize() {
			serialized_blob.clear();
			
			// Create the header, outlined as follows (all entrys are 64 bits)
			//  - Address 0x00: Header length (this is decremented whenever variable data is popped)
			//    - Used to determine the length of a variable object, example for std::vector<int>
			//		uint64_t vector_length = current_variable_length(); // this function needs to read the first 8 bytes as a uint64_t
			//      decrement_variable_length();
			// Todo: Take the original_serialized_blob and use its header, modified, for the new serialized_blob
			
			// Get original header, and append new header data to it
			unsigned int original_header_length;
			if (original_serialized_blob.size() > 0) {
				original_header_length = ReadSerializedBlob(0x00, original_serialized_blob); // Multiply by 8 for the character length
			} else {
				original_header_length = 0;
			}
			unsigned int new_header_length = original_header_length + variable_lengths.size();
			PushToSerializedBlob(static_cast<uint64_t>(new_header_length));
			
			if (original_header_length > 0) {
				for (std::size_t index = 1*8; index < (original_header_length+1)*8; ++index) {
					serialized_blob.push_back(original_serialized_blob[index]);
				}
			}
			for (std::size_t index = 0 ; index < variable_lengths.size(); ++index) {
				PushToSerializedBlob(variable_lengths[variable_lengths.size()-1-index]);
			}
			
			// Append original data minus the old header
			for (std::size_t index = (new_header_length+1)*8; index < original_serialized_blob.size(); ++index) {
				serialized_blob.push_back(original_serialized_blob[index]);
			}
			
			// Append the new data
			auto binary_data_copy = binary_data;
			while(!binary_data_copy.empty()) {
				auto const char_blob = binary_data_copy.top();
				binary_data_copy.pop();
				
				PushBlob(char_blob);
			}
			
			return serialized_blob;
		}
		
		/**
		 * Stores a binary blob of the serialized data to the provided file name and returns the
		 * character blob in a vector.
		 */
		std::vector<unsigned char> Serialize(std::string const& file_name) {
			Serialize();
			
			// Write char array to file
			std::ofstream output(file_name, std::ios::out | std::ios::binary);
			for (auto const& character : serialized_blob) {
				output.put(character);
			}
			
			return serialized_blob;
		}
		
		/**
		 * Resets the values of this class object to whatever contents are in the provided binary blob.
		 */
		void Deserialize(std::string const& file_name) {
			Clear();
		
			std::ifstream input(file_name, std::ios::in | std::ios::binary);
			char character;
			while(input.get(character)) {
				original_serialized_blob.push_back(static_cast<unsigned char>(character));
			}
			
			serialized_blob = original_serialized_blob;
			original_serialized_blob.clear();
		}
		
		/**
		 * Generic push onto queue (calls one of the other specific implementations).
		 * Useful because it enforces more explicit type storage.
		 *
		 * Note: Tag-dispatching is used to allow for STL template specializations
		 */
		template<class T>
		void push(T data) {
			push(tag<T>(), data);
		}
		
		/**
		 * Converts data to a character array and pushes it on top of the data stack.
		 */
		void push(uint64_t const data) {
			std::vector<unsigned char> char_blob;
			for (std::size_t index = 0; index < sizeof(data); ++index) {
				char_blob.push_back((data >> (index*8)) & 0xFF);
			}
			
			binary_data.push(char_blob);
		}
		 
		void push(unsigned int const data) {
			// Data is expanded to 64-bit for compatibility with both x64 and x32
			auto data_64bit = static_cast<uint64_t>(data);
			push(data_64bit);
		}
		
		void push(int const data) {
			auto data_64bit = static_cast<int64_t>(data);
			push(*reinterpret_cast<uint64_t*>(&data_64bit));
		}
		
		void push(unsigned char const data) {
			std::vector<unsigned char> char_blob = {data};
			binary_data.push(char_blob);
		}

		void push(float const data) {
			float data_copy = data;
			uint64_t data_64bit = 0x00;
			data_64bit |= *reinterpret_cast<uint64_t*>(&data_copy);
			push(data_64bit);
		}
		
		void push(double const data) {
			double data_copy = data;
			if (sizeof(double) == 8) {
				push(*reinterpret_cast<uint64_t*>(&data_copy));
			} else {
				uint64_t data_64bit = 0x00;
				data_64bit |= *reinterpret_cast<uint64_t*>(&data_copy);
				push(data_64bit);
			}	
		}
		
		void push(char const* data) {
			std::vector<unsigned char> char_blob;
			char_blob.push_back('\0');
			for (std::size_t index = 0; data[index] != '\0'; ++index) {
				char_blob.push_back(data[index]);
			}
		
			binary_data.push(char_blob);
		}
		
		void push(std::string const& data) {
			char const* char_array = data.c_str();
			push(char_array);
		}
		
		/**
		 * Push specializations for STL containers.
		 */
		//-----------------------------------------------------------------------------
		template<class T1, class T2>
		void push(std::pair<T1, T2> const& data) {
			push<T1>(data.first);
			push<T2>(data.second);
		}
		
		template<class T>
		void push_vector(std::vector<T> const& data_vector) {
			for (auto const& data : data_vector) {
				push(data);
			}
			
			variable_lengths.push_back(data_vector.size());
		}
		
		template<class T1, class T2>
		void push_map(std::map<T1, T2> const& data_map) {
			std::size_t counter = 0;
			for (auto& entry : data_map) {
				push<T1, T2>(entry);
				++counter;
			}
			
			variable_lengths.push_back(counter);
		}
		
		template<class T>
		void push_queue(std::queue<T> data_queue) {
			std::size_t counter = 0;
			while(!data_queue.empty()) {
				push<T>(data_queue.front());
				data_queue.pop();
				++counter;
			}
			
			variable_lengths.push_back(counter);
		}
		
		template<class T>
		void push_stack(std::stack<T> data_stack) {
			// First reverse the stack (due to it being stored in reverse order)
			std::stack<T> stack_reversed;
			while(!data_stack.empty()) {
				stack_reversed.push(data_stack.top());
				data_stack.pop();
			}
		
			std::size_t counter = 0;
			while(!stack_reversed.empty()) {
				push<T>(stack_reversed.top());
				stack_reversed.pop();
				++counter;
			}
			
			variable_lengths.push_back(counter);
		}
		
		/**
		 * Depending on the size of the data type, removes from the char blob data to form data type.
		 */
		template<class T>
		T pop() {
			return pop(tag<T>());
		}
		
		template<class T1, class T2>
		std::pair<T1, T2> pair_pop() {
			T1 first = pop<T1>();
			T2 second = pop<T2>();
			return std::pair<T1, T2>(first, second);
		}
		
		/**
		 * Specialization of pop() for STL containers. All types supported by T pop are supported in the container.
		 */
		template<class T>
		std::vector<T> vector_pop() {
			std::vector<T> data_vector;
			auto length = GetVariableLength();
			for (std::size_t index = 0; index < length; ++index) {
				data_vector.push_back(pop<T>());
			}
			
			DecrementVariableLengthCounter();
			return data_vector;
		}
		
		template<class T1, class T2>
		std::map<T1, T2> map_pop() {
			std::map<T1, T2> data_map;
			auto length = GetVariableLength();
			std::pair<T1, T2> entry;
			for (std::size_t index = 0; index < length; ++index) {
				entry = pop<std::pair<T1, T2>>();
				data_map.insert(entry);
			}
			
			DecrementVariableLengthCounter();
			return data_map;
		}
		
		template<class T>
		std::queue<T> queue_pop() {
			std::queue<T> data_queue;
			auto length = GetVariableLength();
			T entry;
			for (std::size_t index = 0; index < length; ++index) {
				entry = pop<T>();
				data_queue.push(entry);
			}
			
			DecrementVariableLengthCounter();
			return data_queue;
		}
		
		template<class T>
		std::stack<T> stack_pop() {
			std::stack<T> data_stack;
			auto length = GetVariableLength();
			T entry;
			for (std::size_t index = 0; index < length; ++index) {
				entry = pop<T>();
				data_stack.push(entry);
			}
			
			DecrementVariableLengthCounter();
			return data_stack;
		}
	};

	// Template specializations
	//--------------------------------------------------------------------------------------------------------------
	
	/**
	 * Specialization of pop() meant for std::string.
	 * 
	 * Note: Stored in the serial blob as a character array wrapped with a null terminator on both sides.
	 */
	template<>
	std::string SerializeQueue::pop<std::string>() {
		std::vector<unsigned char> char_array;
		for (char character = serialized_blob.back(); character != '\0'; character = serialized_blob.back()) {
			char_array.push_back(character);
			serialized_blob.pop_back();
		}
		serialized_blob.pop_back();
	
		// Create a c string that is the reverse
		char c_string[char_array.size()+1];
		for (int index = char_array.size(); index > 0; --index) {
			c_string[char_array.size() - index] = char_array[index-1];
			
		}
		c_string[char_array.size()] = '\0';
		
		return std::string(c_string);
	}
}	

/**
 * TODO:
 *	- Add support for raw binaries (character blobs that don't use null terminators).
 *  - std::map and other stl containers
 *  - Exception support.
 *		- A better approach may be to use asserts instead, for things like reading beyond the end of the char blob.
 *	- Use move semantics where appropriate to avoid extra copies
 */


#endif // SERIALIZE_QUEUE