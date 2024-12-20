#ifndef _POSITIONSTRUCT_H
#define _POSITIONSTRUCT_H

template <typename T>

struct _Position{
	T x;
	T y;

	_Position(const T& x, const T& y): x(x), y(y){}


	bool operator==(const _Position<T>& other) const {
		return this->x == other.x && this->y == other.y;
	}
	bool operator!=(const _Position<T>& other) const {
		return !(*this == other);
	}
	_Position<T>& operator= (const T & other){
		if (this != other) {
			this->x = other.x;
			this->y = other.y;
		}
		return *this;
	}

	_Position<T>& operator+(const _Position<T>& other) const {
		return _Position<T>(this->x + other.x, this->y + other.y);
	}
	_Position<T>& operator-(const _Position<T>& other) {
		return _Position<T>(this->x - other.x, this->y - other.y);
	}


};
typedef _Position<int> Position;
typedef _Position<float> Vector2;
#endif