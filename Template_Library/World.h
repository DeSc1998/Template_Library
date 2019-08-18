#pragma once

#include <array>
#include <type_traits>

#include "enum.h"

#define Enum( _Enum, _Type, ... ) BETTER_ENUM( _Enum, _Type, __VA_ARGS__ )


namespace ds {

	Enum(default_terrain_, int, none)

	template <
		typename Terrain,
		typename = std::enable_if_t< std::is_enum_v<typename Terrain::_enumerated> >
	>
	struct Tile_traits {
		using terrain_type = Terrain;

		static constexpr size_t num_of_types = terrain_type::_size();
		// assume 'None' is the last element
		static constexpr terrain_type default_terrain = terrain_type::_from_index(num_of_types - 1);
	};


	// default Tile_data
	struct Data {};

	template <
		typename Terrain,
		typename Tile_data = Data,
		typename Traits = Tile_traits<Terrain>
	>
	class Tile {
	public:
		using terrain_type = typename Traits::terrain_type;

		static constexpr size_t num_of_types = Traits::num_of_types;
		static constexpr terrain_type default_terrain = Traits::default_terrain;

	private:
		terrain_type terrain = default_terrain;
		Tile_data data{};

	public:
		Tile() = default;

		template < typename ... Args >
		Tile(terrain_type t, Args&& ... args) : terrain(t), data(args...) {}


		terrain_type& get_terrain() {
			return terrain;
		}

		const terrain_type& get_terrain() const {
			return terrain;
		}

		Tile_data& get_data() {
			return data;
		}

		const Tile_data& get_data() const {
			return data;
		}

		void set_terrain(terrain_type t) {
			terrain = t;
		}

		void set_data(Tile_data&& d) {
			data = d;
		}
	};


	template <
		typename Tile_type = Tile<default_terrain_>,
		size_t X = 10,
		size_t Y = X
	>
	class World {
	public:
		using tile_type      = Tile_type;
		using terrain_type   = typename Tile_type::terrain_type;
		using iterator       = typename std::array< tile_type, X * Y >::iterator;
		using const_iterator = typename std::array< tile_type, X * Y >::const_iterator;

	private:
		std::array< tile_type, X * Y > world;

	public:
		World() = default;


		static bool is_in_world( size_t x, size_t y ) {
			return x >= 0 && x < X && y >= 0 && y < Y;
		}

		tile_type& tile_at( size_t x, size_t y ) {
			return is_in_world(x, y) ? world[x + y * X] : 
				tile_type{};
		}

		const tile_type& tile_at( size_t x, size_t y ) const {
			return is_in_world(x, y) ? world[x + y * X] :
				tile_type{};
		}

		terrain_type terrain_at( size_t x, size_t y ) {
			return is_in_world(x, y) ? 
				world[x + y * X].get_terrain() :
				tile_type::default_terrain;
		}

		terrain_type terrain_at( size_t x, size_t y ) const {
			return is_in_world(x, y) ? 
				world[x + y * X].get_terrain() : 
				tile_type::default_terrain;
		}

		iterator at( size_t x, size_t y ) {
			return ( world.begin() += x + y * X );
		}

		const_iterator at( size_t x, size_t y ) const {
			return ( world.begin() += x + y * X );
		}

		iterator begin() {
			return world.begin();
		}

		const_iterator begin() const {
			return world.begin();
		}

		iterator end() {
			return world.end();
		}

		const_iterator end() const {
			return world.end();
		}

		static constexpr size_t max_x() {
			return X;
		}

		static constexpr size_t max_y() {
			return Y;
		}
	};


	template < typename T, size_t X, size_t Y = X >
	class array_2d : public std::array<T, X*Y> {
	public:
		T& at( size_t x, size_t y ) {
			return (*this)[ x + y * X ];
		}

		const T& at( size_t x, size_t y ) const {
			return (*this)[ x + y * X ];
		}

		static constexpr size_t max_x() {
			return X;
		}

		static constexpr size_t max_y() {
			return Y;
		}
	};

}