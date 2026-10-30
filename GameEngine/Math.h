#ifndef MATH_H
#define MATH_H
//window-directMath , 그외 플랫폼 glm 사용

#if defined(_WIN32) || defined(_WIN64)
#include <DirectXMath.h>
#define ENGINE_MATH_DXMATH
#else 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#if defined(ENGINE_MATH_DXMATH)
#else
#endif

/*
	operator 는 + - 만 지원
	transform , scale 연산은 inline 함수 형태로 지원

	매개 변수의 경우 4byte 단위 경우(int, float 등) const만 붙여서 전달(매개 인자는 값수정 X read only)
	8byte 이상의 경우 vector type 혹은 matrix 타입의 경우
	const Vector_3f& 과 같이 참조 방식 전달 (참조에 8byte 정도만 필요함 즉 8byte 이상 매개 변수부터 비용 아낄 수 있음)
	const Vector_2f& 의 경우 값복사가 참조보다 빠를 수 있으나 통일성을 위해 참조 사용
*/
namespace MJEngine
{

	struct Vector_2i
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMINT2 data;
#else
		glm::ivec2 data;
#endif
		inline Vector_2i() : data{ 0, 0 } {}
		inline Vector_2i(const int x, const int y) 
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMINT2(x, y);
#else
			data = glm::ivec2(x, y);
#endif
		}

		
		inline Vector_2i operator+(const Vector_2i& v) const 
		{
			Vector_2i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt2(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt2(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreSInt2(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_2i operator-(const Vector_2i& v) const
		{
			Vector_2i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt2(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt2(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreSInt2(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}


		inline const int* GetPtr() const
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}


	};

	struct Vector_3i
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMINT3 data;
#else
		glm::ivec3 data;
#endif
		inline Vector_3i() : data{ 0, 0 ,0 } {}
		inline Vector_3i(const int x, const int y, const int z) 
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMINT3(x, y, z);
#else
			data = glm::ivec3(x, y, z);
#endif
		}

		inline Vector_3i operator+(const Vector_3i& v) const
		{
			Vector_3i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt3(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt3(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreSInt3(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_3i operator-(const Vector_3i& v) const
		{
			Vector_3i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt3(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt3(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreSInt3(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}

		inline const int* GetPtr()
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}

	};

	struct Vector_4i
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMINT4 data;
#else
		glm::ivec4 data;
#endif
		inline Vector_4i() : data{ 0, 0 ,0 , 0 } {}
		inline Vector_4i(const int x, const int y, const int z, const int w)
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMINT4(x, y, z, w);
#else
			data = glm::ivec4(x, y, z, w);
#endif
		}

		inline Vector_4i operator+(const Vector_4i& v) const
		{
			Vector_4i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt4(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt4(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreSInt4(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_4i operator-(const Vector_4i& v) const
		{
			Vector_4i result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadSInt4(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadSInt4(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreSInt4(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}
		inline const int* GetPtr()
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}

	};

	struct Vector_2f
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMFLOAT2 data;
#else
		glm::vec2 data;
#endif
		inline Vector_2f() : data{ 0.f, 0.f } {}
		inline Vector_2f(const float x, const float y)
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMFLOAT2(x, y);
#else
			data = glm::vec2(x, y);
#endif
		}

		inline Vector_2f operator+(const Vector_2f& v)
		{
			Vector_2f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreFloat2(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_2f operator-(const Vector_2f& v) const
		{
			Vector_2f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreFloat2(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}

		/*

		외부 함수에서 v를 받을 때 const Vector_2f& v 형태로 상수(const) 참조할시. 해당 함수 내부에서 v는 내부값 변조 불가한 상태여야 한다
		이 상태에서 v * s를 호출하면, 컴파일러는 구조체 내부에서 멤버 변수를 절대 건드리지 않는 안전한(const) 곱셈 연산자를 찾으려고 함
		*/
		inline Vector_2f operator*(const float s) const
		{
			Vector_2f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&data);
			DirectX::XMVECTOR v2 = DirectX::XMVectorScale(v1 , s);
			DirectX::XMStoreFloat2(&result.data, v1);
#else
			result.data = data * s;
#endif
			return result;
		}

		inline const float* GetPtr()
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}

	};

	struct Vector_3f
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMFLOAT3 data;
#else
		glm::vec3 data;
#endif
		inline Vector_3f() : data{ 0.f, 0.f , 0.f } {}
		inline Vector_3f(const float x, const float y, const float z) 
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMFLOAT3(x, y, z);
#else
			data = glm::vec3(x, y, z);
#endif
		}

		inline Vector_3f operator+(const Vector_3f& v) const
		{
			Vector_3f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreFloat3(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_3f operator-(const Vector_3f& v) const
		{
			Vector_3f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreFloat3(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}

		inline Vector_3f operator*(const float s) const
		{
			Vector_3f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&data);
			DirectX::XMVECTOR v2 = DirectX::XMVectorScale(v1, s);
			DirectX::XMStoreFloat3(&result.data, v1);
#else
			result.data = data * s;
#endif
			return result;
		}


		inline const float* GetPtr() const
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}
	};

	struct Vector_4f
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMFLOAT4 data;
#else
		glm::vec4 data;
#endif
		inline Vector_4f() : data{ 0.f, 0.f , 0.f , 0.f } {}
		inline Vector_4f(const float x, const float y, const float z, const float w)
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMFLOAT4(x, y, z, w);
#else
			data = glm::vec4(x, y, z, w);
#endif
		}

		inline Vector_4f operator+(const Vector_4f& v) const
		{
			Vector_4f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorAdd(v1, v2);
			DirectX::XMStoreFloat4(&result.data, v3);
#else
			result.data = data + v.data;
#endif

			return result;
		}

		inline Vector_4f operator-(const Vector_4f& v) const
		{
			Vector_4f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&data);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&v.data);
			DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(v1, v2);
			DirectX::XMStoreFloat4(&result.data, v3);
#else
			result.data = data - v.data;
#endif

			return result;
		}

		inline Vector_4f operator*(const float s) const
		{
			Vector_4f result;

#if defined(ENGINE_MATH_DXMATH)
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&data);
			DirectX::XMVECTOR v2 = DirectX::XMVectorScale(v1, s);
			DirectX::XMStoreFloat4(&result.data, v1);
#else
			result.data = data * s;
#endif
			return result;
		}

		inline const float* GetPtr() const
		{
#if defined(ENGINE_MATH_DXMATH)
			return &data.x;
#else
			return glm::value_ptr(data);
#endif
		}
	};


	//vector 연산 

	inline Vector_2f operator*(const float s ,const Vector_2f& v)
	{
		return v * s;
	}

	inline Vector_3f operator*(const float s, const Vector_3f& v)
	{
		return v * s;
	}

	inline Vector_4f operator*(const float s, const Vector_4f& v)
	{
		return v * s;
	}


	inline float Dot_2f(const Vector_2f& v1, const Vector_2f& v2)
	{
		float result = 0;
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat2(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat2(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector2Dot(xmV1, xmV2);
		DirectX::XMStoreFloat(&result, xmV3);
#else
		result = glm::dot(v1.data, v2.data);
#endif
		return result;
	}

	inline float Dot_3f(const Vector_3f& v1, const Vector_3f& v2)
	{
		float result = 0;
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat3(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat3(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector3Dot(xmV1, xmV2);
		DirectX::XMStoreFloat(&result, xmV3);
#else
		result = glm::dot(v1.data, v2.data);
#endif
		return result;
	}

	inline float Dot_4f(const Vector_4f& v1, const Vector_4f& v2)
	{
		float result = 0;
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat4(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat4(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector4Dot(xmV1, xmV2);
		DirectX::XMStoreFloat(&result, xmV3);
#else
		result = glm::dot(v1.data, v2.data);
#endif
		return result;
	}

	inline float Cross_2f(const Vector_2f& v1, const Vector_2f& v2)
	{
		float result = 0;
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat2(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat2(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector2Cross(xmV1, xmV2);
		DirectX::XMStoreFloat(&result, xmV3);
#else
		result = glm::cross(v1.data, v2.data);
#endif
		return result;
	}

	inline Vector_3f Cross_3f(const Vector_3f& v1, const Vector_3f& v2)
	{
		Vector_3f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat3(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat3(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector3Cross(xmV1, xmV2);
		DirectX::XMStoreFloat3(&result.data, xmV3);
#else
		result.data = glm::cross(v1.data, v2.data);
#endif
		return result;
	}

	// 3차원상의 외적만을 지원함 
	inline Vector_4f Cross3D_4f(const Vector_4f& v1, const Vector_4f& v2)
	{
		Vector_4f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat4(&v1.data);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat4(&v2.data);
		DirectX::XMVECTOR xmV3 = DirectX::XMVector3Cross(xmV1, xmV2);
		DirectX::XMStoreFloat4(&result.data, xmV3);
#else
		glm::vec3 temp = glm::cross(v1.data, v2.data);
		result.data = glm::vec4(temp, 0.f);
#endif
		return result;
	}

	inline Vector_2f Normalize_2f(const Vector_2f& v)
	{
		Vector_2f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat2(&v.data);
		DirectX::XMVECTOR xmv2 = DirectX::XMVector2Normalize(xmv1);
		DirectX::XMStoreFloat2(&result.data, xmv2);
#else
		result.data = glm::normalize(v.data);
#endif
		return result;
	}


	inline Vector_3f Normalize_3f(const Vector_3f& v)
	{
		Vector_3f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.data);
		DirectX::XMVECTOR xmv2 = DirectX::XMVector3Normalize(xmv1);
		DirectX::XMStoreFloat3(&result.data, xmv2);
#else
		result.data = glm::normalize(v.data);
#endif
		return result;
	}


	inline Vector_4f Normalize_4f(const Vector_4f& v)
	{
		Vector_4f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v.data);
		DirectX::XMVECTOR xmv2 = DirectX::XMVector4Normalize(xmv1);
		DirectX::XMStoreFloat4(&result.data, xmv2);
#else
		result.data = glm::normalize(v.data);
#endif
		return result;
	}


	inline float LengthSq_2f(const Vector_2f& v)
	{
		return Dot_2f(v, v);
	}

	inline float LengthSq_3f(const Vector_3f& v)
	{
		return Dot_3f(v, v);
	}

	inline float LengthSq3D_4f(const Vector_4f& v)
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v.data);
		return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(xmv1));
#else
		return v.data.x * v.data.x + v.data.y * v.data.y + v.data.z * v.data.z;
#endif

	}

	/*
	* matrix 는 16바이트 정렬의(16바이트 단위로 padding 을 통해서라도 주소체계를 맞추어 정렬 시킴)
	* DirectX::XMMATRIX data 으로 관리하기 위해서
	* float 4 X 4 형태로만 관리함
	* 변환시 3x3 이나 이하 차수의 행렬의 변환은 대각 요소를 1로 만들어줌으로 통일하여 결국 mat4 에 대한 연산이 이뤄지게 한다.
	* transpose
	* inverse
	* determinant
	* tranform
	* 등 요구됨
	*/
	struct alignas(16) Matrix_4fA
	{
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMMATRIX data;
#else
		glm::mat4 data;
#endif

		inline Matrix_4fA()
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMMatrixIdentity();
#else
			data = glm::mat4(1.0f);
#endif
		}
		// 행 기반 벡터형태 입력으로 처리
		inline Matrix_4fA(
			const Vector_4f& row0, const Vector_4f& row1, const Vector_4f& row2, const Vector_4f& row3
		)
		{
#if defined(ENGINE_MATH_DXMATH)
			data.r[0] = DirectX::XMLoadFloat4(&row0.data);
			data.r[1] = DirectX::XMLoadFloat4(&row1.data);
			data.r[2] = DirectX::XMLoadFloat4(&row2.data);
			data.r[3] = DirectX::XMLoadFloat4(&row3.data);
#else
			mat4 tempData(row0.data, row1.data, row2.data, row3.data);
			data = glm::transpose(tempData);
#endif 
		}

		inline Matrix_4fA(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33
		)
		{
#if defined(ENGINE_MATH_DXMATH)
			data = DirectX::XMMATRIX(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33
			);
#else
			data = glm::mat4(
				m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33
			);
#endif
		}

	};


	inline Vector_4f transform_4f(const Vector_4f& v , const Matrix_4fA& m)
	{
		Vector_4f result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR xm1 = DirectX::XMLoadFloat4(&v.data);
		DirectX::XMVECTOR xm2 = DirectX::XMVector4Transform( xm1 , m.data );
		DirectX::XMStoreFloat4(&result.data, xm2 );
#else
		result.data = m.data * v.data;
#endif
		return result;
	}

	inline Matrix_4fA transpose_4f(const Matrix_4fA& m)
	{
		Matrix_4fA result = {};

#if defined(ENGINE_MATH_DXMATH)
		result.data = DirectX::XMMatrixTranspose(m.data);
#else
		result.data = glm::transpose(m.data);
#endif
		return result;
	}
	
	inline Matrix_4fA inverse(const Matrix_4fA& m)
	{
		Matrix_4fA result = {};
#if defined(ENGINE_MATH_DXMATH)
		DirectX::XMVECTOR detM = DirectX::XMMatrixDeterminant(m.data);
		assert(DirectX::XMVectorGetX(detM)  != 0 && "Detertnant zero! ");
		result.data = DirectX::XMMatrixInverse( &detM , m.data);
#else
		float detA = glm::determinant(m.data);
		assert(detA.x != 0 && "Detertnant zero! ");
		result.data = glm::inverse(m.data);

#endif
	}
	/*
	이후 필요할 때 마다 구현
	기하학 함수 Geometry
	삼각형-점 거리
	라인 - 점 거리
	점 - 점 거리
	*/
}
#endif // !MATH_H
