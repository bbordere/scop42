#pragma once

#include <Vectors.hpp>
#include <cstring>
#include <iomanip>
#include <iostream>

template <class T>
class Mat4 {
	private:
	public:
		T data[16] = {T()};

	public:
		Mat4() {}

		static Mat4 makeIdentity() {
			Mat4 res;
			res.data[0] = static_cast<T>(1);
			res.data[5] = static_cast<T>(1);
			res.data[10] = static_cast<T>(1);
			res.data[15] = static_cast<T>(1);
			return (res);
		}

		static Mat4 makePerspective(T fovY, T aspect, T near, T far) {
			Mat4 res;
			T h = std::tan(((fovY * M_PI) / 180) / 2);
			res.data[0] = static_cast<T>(1) / (aspect * h);
			res.data[5] = static_cast<T>(1) / h;
			res.data[10] = -(far + near) / (far - near);
			res.data[11] = -static_cast<T>(1);
			res.data[14] = -(static_cast<T>(2) * far * near) / (far - near);
			return (res);
		}

		static Mat4 makeOrthogonal(T left, T right, T bot, T top, T near,
								   T far) {
			Mat4 res;
			Vector3<T> vec;
			vec.x = static_cast<T>(-(right + left) / (right - left));
			vec.y = static_cast<T>(-(top + bot) / (top - bot));
			vec.z = static_cast<T>(-(far + near) / (far - near));
			res.data[0] = static_cast<T>(2 / (right - left));
			res.data[5] = static_cast<T>(2 / (top - bot));
			res.data[10] = static_cast<T>(-2 / (far - near));
			res.data[12] = vec.x;
			res.data[13] = vec.y;
			res.data[14] = vec.z;
			res.data[15] = 1;
			return (res);
		}

		static Mat4 zebi(int f) {
			Mat4 res;
			if (f) {
				for (int i = 0; i < 16; ++i) {
					res.data[i] = i;
				}
			}
			else {
				for (int i = 15; i >= 0; --i) {
					res.data[15 - i] = i;
				}
			}
			return (res);
		}

		static Mat4 lookAt(Vector3<T> const &eye, Vector3<T> const &center,
						   Vector3<T> const &up) {
			Vector3 f = Vector3<T>::normalize(center - eye);
			Vector3 u = Vector3<T>::normalize(up);
			Vector3 s = Vector3<T>::normalize(Vector3<T>::cross(f, u));
			u = Vector3<T>::cross(s, f);

			Mat4 res = Mat4::makeIdentity();
			res.data[0 * 4 + 0] = static_cast<T>(s.x);
			res.data[1 * 4 + 0] = static_cast<T>(s.y);
			res.data[2 * 4 + 0] = static_cast<T>(s.z);
			res.data[0 * 4 + 1] = static_cast<T>(u.x);
			res.data[1 * 4 + 1] = static_cast<T>(u.y);
			res.data[2 * 4 + 1] = static_cast<T>(u.z);
			res.data[0 * 4 + 2] = static_cast<T>(-f.x);
			res.data[1 * 4 + 2] = static_cast<T>(-f.y);
			res.data[2 * 4 + 2] = static_cast<T>(-f.z);
			res.data[3 * 4 + 0] = static_cast<T>(-Vector3<T>::dot(s, eye));
			res.data[3 * 4 + 1] = static_cast<T>(-Vector3<T>::dot(u, eye));
			res.data[3 * 4 + 2] = static_cast<T>(Vector3<T>::dot(f, eye));
			return res;
		}

		Mat4(Mat4 const &copy) {
			std::memmove(this->data, copy.data, 16 * sizeof(T));
		}

		T const *getDataPtr() const {
			return (&this->data[0]);
		}

		Mat4 &operator*=(Mat4 const &other) {
			T buff[16] = {0};
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					for (int k = 0; k < 4; ++k) {
						buff[i * 4 + j] +=
							this->data[i * 4 + k] * other.data[k * 4 + j];
					}
				}
			}
			std::memmove(this->data, buff, sizeof(buff));
			return (*this);
		}

		// static Vector3<T> scaling(Vector3<T> const &vec) {
		// 	return (Vector3(this->data[0] * vec.x, this->data[1] * vec.y,
		// 					this->data[2] * vec.z));
		// }

		void translate(Vector3<T> const &vec) {
			this->data[12] += vec.x;
			this->data[13] += vec.y;
			this->data[14] += vec.z;
		}

		void rotate(float angle, Vector3<T> const &caxis) {
			angle = (angle * M_PI) / 180;

			Vector3 axis = caxis.normalize();
			float c = std::cos(angle);
			float s = -std::sin(angle);
			T x = axis.x;
			T y = axis.y;
			T z = axis.z;

			Mat4 rota;
			rota.data[0] = x * x * (1 - c) + c;
			rota.data[1] = x * y * (1 - c) - z * s;
			rota.data[2] = x * z * (1 - c) + y * s;
			rota.data[3] = 0;
			rota.data[4] = y * x * (1 - c) + z * s;
			rota.data[5] = y * y * (1 - c) + c;
			rota.data[6] = y * z * (1 - c) - x * s;
			rota.data[7] = 0;
			rota.data[8] = z * x * (1 - c) - y * s;
			rota.data[9] = z * y * (1 - c) + x * s;
			rota.data[10] = z * z * (1 - c) + c;
			rota.data[11] = 0;
			rota.data[12] = 0;
			rota.data[13] = 0;
			rota.data[14] = 0;
			rota.data[15] = 1;

			*this *= rota;
		}

		void transformMat3() {
			this->data[3] = static_cast<T>(0);
			this->data[7] = static_cast<T>(0);
			this->data[11] = static_cast<T>(0);
			this->data[12] = static_cast<T>(0);
			this->data[13] = static_cast<T>(0);
			this->data[14] = static_cast<T>(0);
			this->data[15] = static_cast<T>(0);
		}

		Mat4 operator*(Mat4 const &other) {
			Mat4 res;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					for (int k = 0; k < 4; ++k) {
						res.data[j * 4 + i] +=
							this->data[k * 4 + i] * other.data[j * 4 + k];
					}
				}
			}
			return (res);
		}

		void scale(vec3f const &scale) {
			this->data[0] *= scale.x;
			this->data[5] *= scale.y;
			this->data[10] *= scale.z;
		}
};

template <typename T>
std::ostream &operator<<(std::ostream &output, Mat4<T> const &mat) {
	for (int i = 0; i < 16; i += 4) {
		output << "| " << mat.getDataPtr()[i] << ' ' << mat.getDataPtr()[i + 1]
			   << ' ' << mat.getDataPtr()[i + 2] << ' '
			   << mat.getDataPtr()[i + 3] << "|\n";
	}
	return (output);
}

typedef Mat4<float> mat4f;
