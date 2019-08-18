#pragma once

#include <algorithm>
#include <functional>
#include <type_traits>
#include <memory>

#include "../Nodes/node.h"


namespace ds {

	template < typename Type >
	class binarytree {

		std::less<Type> compare;

		using node_type = duo_node<Type>;

		unsigned int  num_elem = 0;
		std::shared_ptr<node_type> root;
		
	public:
		binarytree() = default;

		binarytree( const Type& t ) {
			root = std::make_shared<node_type>(t);
			++num_elem;
		}


		void insert( const Type& t ) {
			if ( !root ) {
				root = std::make_shared<node_type>(t);
				++num_elem;
			} else {
				compare( t, root->value ) ?
					insert_left( t, root ) :
					insert_right( t, root );
			}
		}

		bool find( const Type& t ) const {
			auto ptr = root;

			while ( ptr ) { // ptr != nullptr
				if (t == ptr->value)
					return true;

				compare( t, ptr->value ) ?
					ptr = ptr->prev :
					ptr = ptr->next;
			}
			return false;
		}

		void remove( const Type& t ) {
			// TODO
		}

		void clear() {
			root.reset();
			num_elem = 0;
		}

		bool is_empty() {
			return !root;
		}

		void balance() {
			const auto b = balance_of(root);

			if ( b > 1 ) {
				rotate_left( root->get_prev(), root->get_prev()->get_next() );
				rotate_right( root, root->get_prev() );
			} else if ( b < -1 ) {
				rotate_right( root->get_next(), root->get_next()->get_prev() );
				rotate_left( root, root->get_next() );
			}
		}


	private:
		static int balance_of( const std::shared_ptr<node_type>& ptr ) {
			return (int)height( ptr->prev ) - (int)height( ptr->next );
		}

		static size_t height( const std::shared_ptr<node_type>& ptr ) {
			return ptr ? 
				std::max( 
					height( ptr->get_next() ), 
					height( ptr->get_prev() ) 
				) + 1
				: 1;
		}

		void insert_left( const Type& t, std::shared_ptr<node_type>& node ) {
			auto&& left = node->prev;

			if ( !left ) { // left == nullptr
				left = std::make_shared<node_type>(t);
				++num_elem;
			} else {
				compare(t, left->value) ?
					insert_left(t, left) :
					insert_right(t, left);
			}
		}

		void insert_right( const Type& t, std::shared_ptr<node_type>& node ) {
			auto&& right = node->next;

			if ( !right ) { // right == nullptr
				right = std::make_shared<node_type>(t);
				++num_elem;
			} else {
				compare(t, right->value) ?
					insert_left(t, right) :
					insert_right(t, right);
			}
		}

		static void rotate_left( std::shared_ptr<node_type>& top, std::shared_ptr<node_type>& k ) {
			//       top         //
			//        |          //
			//        k          //
			//      /   \        //
			//         k_right   //
			//          /    \   //
			//       r_left      //

			auto k_right = k->next;
			auto r_left = k_right->prev;


			k->next = r_left;
			
			if (top->next == k)
				top->next = k_right;
			else
				top->prev = k_right;

			k_right->prev = k;
		}

		static void rotate_right( std::shared_ptr<node_type>& top, std::shared_ptr<node_type>& k ) {
			//         top       //
			//          |        //
			//          k        //
			//        /   \      //
			//    k_left         //
			//    /    \         //
			//        l_right    //

			auto k_left = k->prev;
			auto l_right = k_left->next;


			k->prev = l_right;

			if ( top->next == k )
				top->next = k_left;
			else 
				top->prev = k_left;

			k_left->next = k;
		}

	};

}

