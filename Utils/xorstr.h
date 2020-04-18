////////////////////////////////////////////////////////////////////
template <int X>
struct EnsureCompileTime {
	enum : int {
		Value = X
	};
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//Use Compile-Time as seed
#define XorSeed ((__TIME__[7] - '0') * 1 + (__TIME__[6] - '0') * 10 +   \
			  (__TIME__[4] - '0') * 60 + (__TIME__[3] - '0') * 600 + \
			  (__TIME__[1] - '0') * 3600 + (__TIME__[0] - '0') * 36000)
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
constexpr int LinearCongruentGenerator(int Rounds) {
	return 1013904223 + 1664525 * ((Rounds > 0) ? LinearCongruentGenerator(Rounds - 1) : XorSeed & 0xFFFFFFFF);
}
#define Random() EnsureCompileTime<LinearCongruentGenerator(10)>::Value  //10 Rounds
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
template <int... Pack>
struct IndexList {};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
template <typename IndexList, int Right>
struct Append;
template <int... Left, int Right>
struct Append<IndexList<Left...>, Right> {
	typedef IndexList<Left..., Right> Result;
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
template <int N>
struct ConstructIndexList {
	typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};
template <>
struct ConstructIndexList<0> {
	typedef IndexList<> Result;
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
const char XORKEY = static_cast<char>(RandomNumber(0, 0xFF));
__forceinline constexpr char EncryptCharacter(const char Character, int Index) {
	return Character ^ (XORKEY + Index);
}

template <typename IndexList>
class CXorString;
template <int... Index>
class CXorString<IndexList<Index...> > {
private:
	volatile char Value[sizeof...(Index) + 1];

public:
	__forceinline constexpr CXorString(const char* const String)
		: Value{EncryptCharacter(String[Index], Index)...} {}

	__forceinline volatile char* decrypt() {
		for (int t = 0; t < sizeof...(Index); t++) {
			Value[t] = Value[t] ^ (XORKEY + t);
		}
		Value[sizeof...(Index)] = '\0';
		return Value;
	}

	__forceinline char* get() {
		return Value;
	}
};
#define XorS(X, String) CXorString<ConstructIndexList<sizeof(String) - 1>::Result> X(String)
#define XorString(String) (CXorString<ConstructIndexList<sizeof(String) - 1>::Result>(String).decrypt())
//#define XorString(String) String