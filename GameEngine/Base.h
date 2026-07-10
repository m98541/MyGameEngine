#ifndef BASE_H
#define BASE_H

#include <memory>



namespace MJEngine
{
	template<typename T>
	using ScopePtr = std::unique_ptr<T>;
	template<typename T , typename ... Args>
	constexpr ScopePtr<T> CreateScopePtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	//2026.06.30
	/*
		현재는 따로 heap allocate 및 참조 카운트로 관리하는 포인터가 없는 관계로
		std lib 의 shared_ptr 사용 => 이후 성능을 위해 제거 필요
	*/
	template<typename T>
	using RefPtr = std::shared_ptr<T>;
	template<typename T , typename ... Args>
	constexpr RefPtr<T> CreatRefPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}



}

#endif // !BASE_H