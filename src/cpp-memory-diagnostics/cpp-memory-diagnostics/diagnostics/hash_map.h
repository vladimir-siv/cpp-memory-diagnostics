#pragma once

#ifdef __MEMORY_DIAGNOSTICS__

#include <exception>
#include <cstdint>

namespace collections
{
	class invalidkeyexception : public std::exception
	{
		public: virtual const char* what() const override
		{
			return "Key is not valid.";
		}
	};
	class keyalreadyexistsexception : public std::exception
	{
		public: virtual const char* what() const override
		{
			return "Key already exists.";
		}
	};
	class keynotfoundexception : public std::exception
	{
		public: virtual const char* what() const override
		{
			return "Key not found.";
		}
	};
	class collectionfullexception : public std::exception
	{
		public: virtual const char* what() const override
		{
			return "Collection is full.";
		}
	};

	template <typename TKey, typename TVal, uint64_t __size = 983ul, uint64_t __bucket_size = 64ul>
	class hash_map final
	{
		private: template <typename TKey, typename TVal> struct hash_pair
		{
			TKey key;
			TVal val;
			hash_pair() : key(nullptr), val{} { }
			//hash_pair(TKey _key, TVal _val) : key(_key), val(_val) { }
		};
		
		private: template <typename TKey, typename TVal> class hash_vector
		{
			private: hash_pair<TKey, TVal> vec[__bucket_size];
			
			public: void insert(TKey key, TVal val)
			{
				if (key == nullptr) throw invalidkeyexception();

				uint64_t index = __bucket_size;

				for (uint64_t i = 0; i < __bucket_size; ++i)
				{
					auto& slot = vec[i];
					if (slot.key == key) throw keyalreadyexistsexception();
					if (slot.key == nullptr && index == __bucket_size) index = i;
				}

				if (index == __bucket_size) throw collectionfullexception();

				auto& slot = vec[index];
				slot.key = key;
				slot.val = val;
			}
			public: TVal find(TKey key)
			{
				if (key == nullptr) throw invalidkeyexception();

				for (uint64_t i = 0; i < __bucket_size; ++i)
				{
					auto& slot = vec[i];

					if (slot.key == key)
					{
						return slot.val;
					}
				}

				throw keynotfoundexception();
			}
			public: TVal remove(TKey key)
			{
				if (key == nullptr) throw invalidkeyexception();

				for (uint64_t i = 0; i < __bucket_size; ++i)
				{
					auto& slot = vec[i];

					if (slot.key == key)
					{
						slot.key = nullptr;
						return slot.val;
					}
				}

				throw keynotfoundexception();
			}
		};
		
		private: hash_vector<TKey, TVal> _map[__size];
		private: uint64_t _count = 0;
		
		private: uint64_t _hash(TKey key)
		{
			if (key == nullptr) throw invalidkeyexception();
			uint64_t hvalue = reinterpret_cast<uint64_t>(key);
			if (__size > 100) hvalue /= 100;
			if (hvalue > __size) hvalue %= __size;
			return hvalue;
		}
		
		public: uint64_t count() const { return _count; }
		
		public: void insert(TKey key, TVal val)
		{
			auto& bucket = _map[_hash(key)];
			bucket.insert(key, val);
			++_count;
		}
		public: TVal find(TKey key)
		{
			auto& bucket = _map[_hash(key)];
			return bucket.find(key);
		}
		public: TVal remove(TKey key)
		{
			auto& bucket = _map[_hash(key)];
			TVal v = bucket.remove(key);
			--_count;
			return v;
		}
	};
}

#endif
