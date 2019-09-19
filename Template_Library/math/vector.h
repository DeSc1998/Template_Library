
#pragma once

#include <iterator>
#include <memory>

namespace ds {

	template < typename Type >
	class vector {
		static const Type default_value = Type{ 0u };

	public:
		
		class iterator : public std::iterator<std::bidirectional_iterator_tag, Type> {
		protected:

			Type* ptr;

		public:
			iterator() = default;

			iterator( Type* _ptr ) : ptr(_ptr) {}


			// Derefernzierung
			Type& operator* () {
				return *this->ptr;
			}

			// Derefernzierung
			const Type& operator* () const {
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
		std::unique_ptr<Type[]> elements = std::make_unique<Type[]>( Size );

	public:
		// Konstructoren
		vector() = default;

		explicit vector( const Type& ref ) {
			elements = std::make_unique<Type[]>( 10 );
			elements.init();
			elements[0] = ref;
			Size = 10;
			num_elements++;
		}

		vector( const vector& other ) {
			elements = std::make_unique<Type[]>( other.Size );
			vector::copy( other.begin(), other.end(), begin() );
			Size = other.Size;
			num_elements = other.num_elements;
		}

		// ändern der Größe
		void resize( size_t new_size ) {
			auto temp = std::make_unique<Type[]>( new_size );
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

		const Type& at( size_t index ) const {
			if ( index < this->Size )
				return elements[index];
			else
				return ;
		}

		Type& at( size_t index ) {
			if ( index < this->Size )
				return elements[index];
			else
				return ;
		}

		const Type& operator[] ( size_t index ) const {
			return elements[index];
		}

		Type& operator[] ( size_t index ) {
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
			return iterator( const_cast<Type*>(&elements[0]) ) ;
		}

		// Iterator zeigt auf den Platz hinter dem letzten Element
		iterator end() const {
			return iterator( const_cast<Type*>(&elements[num_elements]) );
		}
	};

}