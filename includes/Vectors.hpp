#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>

template <typename T>
class Vector3 {

	public:
		T x;
		T y;
		T z;

	public:
		Vector3(): x(T()), y(T()), z(T()) {}

		Vector3(T x, T y, T z): x(x), y(y), z(z) {}

		Vector3(Vector3 const &copy): x(copy.x), y(copy.y), z(copy.z) {}

		Vector3<T> &operator=(Vector3<T> const &assign) {
			this->x = assign.x;
			this->y = assign.y;
			this->z = assign.z;
			return (*this);
		}

		Vector3 operator+(Vector3 const &other) const {
			return (Vector3(this->x + other.x, this->y + other.y,
							this->z + other.z));
		}

		void operator+=(Vector3 const &other) {
			*this = *this + other;
		}

		Vector3 operator+(int x) const {
			return (Vector3(this->x + x, this->y + x, this->z + x));
		}

		Vector3 operator-(Vector3 const &other) const {
			return (Vector3(static_cast<T>(this->x - other.x),
							static_cast<T>(this->y - other.y),
							static_cast<T>(this->z - other.z)));
		}

		Vector3 operator-(int n) const {
			return (Vector3(static_cast<T>(this->x - n),
							static_cast<T>(this->y - n),
							static_cast<T>(this->z - n)));
		}

		void operator-=(Vector3 const &other) {
			*this = *this - other;
		}

		Vector3 cross(Vector3 const &other) const {
			return (Vector3((this->y * other.z) - (this->z * other.y),
							(this->z * other.x) - (this->x * other.z),
							(this->x * other.y) - (this->y * other.x)));
		}

		template <typename U>
		Vector3 operator*(U x) const {
			return (Vector3(static_cast<T>(this->x * x),
							static_cast<T>(this->y * x),
							static_cast<T>(this->z * x)));
		}

		Vector3 operator*(Vector3<T> other) const {
			return (Vector3(static_cast<T>(this->x * other.x),
							static_cast<T>(this->y * other.y),
							static_cast<T>(this->z * other.z)));
		}

		template <typename U>
		Vector3 operator/(U x) const {
			return (Vector3(this->x / static_cast<T>(x),
							this->y / static_cast<T>(x),
							this->z / static_cast<T>(x)));
		}

		double len() const {
			return (std::sqrt((this->x * this->x) + (this->y * this->y) +
							  (this->z * this->z)));
		}

		double dot(Vector3 const &other) const {
			return ((this->x * other.x) + (this->y * other.y) +
					(this->z * other.z));
		}

		Vector3 normalize() const {
			double len = this->len();
			return (Vector3(this->x / len, this->y / len, this->z / len));
		}

		static double dot(Vector3<T> const &v1, Vector3<T> const &v2) {
			return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
		}

		static Vector3 normalize(Vector3<T> const &v) {
			double len = v.len();
			return (Vector3(v.x / len, v.y / len, v.z / len));
		}

		static Vector3 cross(Vector3<T> const &v1, Vector3<T> const &v2) {
			return (Vector3((v1.y * v2.z) - (v1.z * v2.y),
							(v1.z * v2.x) - (v1.x * v2.z),
							(v1.x * v2.y) - (v1.y * v2.x)));
		}

		friend bool operator==(Vector3<T> const &l, Vector3<T> const &r) {
			return (l.x == r.x && l.y == r.y && l.z == r.z);
		}

		friend bool operator!=(Vector3<T> const &l, Vector3<T> const &r) {
			return (!(l == r));
		}

		friend bool operator<(Vector3<T> const &l, Vector3<T> const &r) {
			return (l.len() < r.len());
		}
};

template <typename T>
std::ostream &operator<<(std::ostream &output, Vector3<T> const &vec) {
	output << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
	return (output);
}

template <typename T>
class Vector2 {

	public:
		T x;
		T y;

	public:
		Vector2(): x(T()), y(T()) {}

		Vector2(T x, T y): x(x), y(y) {}

		Vector2(Vector2 const &copy): x(copy.x), y(copy.y) {}

		Vector2<T> &operator=(Vector2<T> const &assign) {
			this->x = assign.x;
			this->y = assign.y;
			return (*this);
		}

		Vector2 operator+(Vector2 const &other) const {
			return (Vector2(this->x + other.x, this->y + other.y));
		}

		Vector2 operator+(int x) const {
			return (Vector2(this->x + x, this->y + x));
		}

		Vector2 operator-(Vector2 const &other) const {
			return (Vector2(this->x - other.x, this->y - other.y));
		}

		Vector2 operator-(int x) const {
			return (Vector2(this->x - x, this->y - x));
		}

		Vector2 operator*(int x) const {
			return (Vector2(this->x * x, this->y * x));
		}

		Vector2 operator/(double x) const {
			return (Vector2(this->x / x, this->y / x));
		}

		double dot(Vector2 const &other) const {
			return ((this->x * other.x) + (this->y * other.y));
		}

		double len() const {
			return (std::sqrt((this->x * this->x) + (this->y * this->y)));
		}

		Vector2 normalize() const {
			double len = this->len();
			return (Vector2(this->x / len, this->y / len));
		}
};

template <class T>
bool operator==(Vector2<T> const &l, Vector2<T> const &r) {
	return (l.x == r.x && l.y == r.y);
}

template <class T>
bool operator!=(Vector2<T> const &l, Vector2<T> const &r) {
	return (!(l == r));
}

template <typename T>
std::ostream &operator<<(std::ostream &output, Vector2<T> const &vec) {
	output << '(' << vec.x << ", " << vec.y << ')';
	return (output);
}

typedef Vector3<float> vec3f;
typedef Vector3<int> vec3i;
typedef Vector3<unsigned int> vec3u;

typedef Vector2<float> vec2f;
typedef Vector2<int> vec2i;
typedef Vector2<unsigned int> vec2u;
