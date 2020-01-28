
#pragma once

#include <iterator>
#include <memory>

namespace ds {

	template < typename T >
	class vector {
		static const T default_value = T{ 0u };

	public:
		
		class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
		protected:

			T* ptr;

		public:
			iterator() = default;

			iterator( T* _ptr ) : ptr(_ptr) {}


			// Derefernzierung
			T& operator* () {
				return *this->ptr;
			}

			// Derefernzierung
			const T& operator* () const {
				return *this->ptr;
			}

			bool operator== ( const iterator& other ) const {
				return this->ptr == other.ptr;
			}

			bool operator!= ( const iterator& other ) const {
				return !( *this == other );
			}

			// Inkrement
			virtual iterator& operator++ () {
				this->ptr++;
				return *this;
			}

			// Dekrement
			virtual iterator& operator-- () {
				this->ptr--;
				return *this;
			}
		};

	private:

		static void copy( iterator _begin, const iterator _end, iterator begin) {
			while ( _begin != _end ) {
				*begin = *_begin;
				++_begin; 
				++begin;
			}
		}

		size_t Size = 10, num_elements = 0;
		std::unique_ptr<T[]> elements = std::make_unique<T[]>( Size );

	public:
		// Konstructoren
		vector() = default;

		explicit vector( const Type& ref ) {
			elements = std::make_unique<T[]>( 10 );
			elements.init();
			elements[0] = ref;
			Size = 10;
			num_elements++;
		}

		vector( const vector& other ) {
			elements = std::make_unique<T[]>( other.Size );
			vector::copy( other.begin(), other.end(), begin() );
			Size = other.Size;
			num_elements = other.num_elements;
		}

		// ändern der Größe
		void resize( size_t new_size ) {
			auto temp = std::make_unique<T[]>( new_size );
			vector::copy( begin(), end(), iterator( &(*temp)) );
			elements = std::move( temp );
			Size = new_size;
		}

		// einfügen von Elementen
		void push_back( const Type& ref ) {
			bool isPlaced = false;
			for ( size_t i = 0; i < Size; i++ ) {
				if ( elements[i] == default_value) {
					elements[i] = ref;
					num_elements++;
					isPlaced = true;
					break;
				}
			}

			if ( !isPlaced ) {
				resize( Size + 10 );
				elements[Size - 10] = ref;
				num_elements++;
			}
		}

		// Vectorinhalt und -manipulation
		size_t size() {
			return Size;
		}

		const T& at( size_t index ) const {
			if ( index < this->Size )
				return elements[index];
			else
				return ;
		}

		T& at( size_t index ) {
			if ( index < this->Size )
				return elements[index];
			else
				return ;
		}

		const T& operator[] ( size_t index ) const {
			return elements[index];
		}

		T& operator[] ( size_t index ) {
			return elements[index];
		}

		// löschen aller Elemente
		static void clear( vector& v ) {
			for ( iterator iter = v.begin(); iter != v.end(); ++iter ) {
				*iter = default_value;
			}
		}

		// -- Iteratoren --
		// Iterator zeigt auf Anfang
		iterator begin() const {
			return iterator( const_cast<T*>(&elements[0]) ) ;
		}

		// Iterator zeigt auf den Platz hinter dem letzten Element
		iterator end() const {
			return iterator( const_cast<T*>(&elements[num_elements]) );
		}
	};

}