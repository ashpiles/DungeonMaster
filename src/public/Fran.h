#include <functional>

// Functional Reactive Animation
namespace FRAN
{
	// A function of time returning any real number, or in our case T
	template <typename T>
	using Behavior = std::function<T(float)>;

	// A Behavior which does not change its value over time
	template <typename T>
	Behavior<T> Constant(const T& Value)
	{
		return [Value](float) { return Value; };
	}

	// Identity time Behavior
	//inline TBehavior<float> Time = [](float Time) {return Time;};

	// The Lifting function turns all parameters of a function into Behaviors
	template <typename F, typename... B>
	auto Lift(F Func, B... Behaviors)
	{
		return [=](float Time)
		{
			return Func(Behaviors(Time)...);
		};
	}
}

