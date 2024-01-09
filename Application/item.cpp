#include"item.h"

Vector3 Esing(const Vector3& st, const Vector3& ed, const float t) {

	return{

		st.x * (1.0f - t) + ed.x * t,
		st.y * (1.0f - t) + ed.y * t,
		st.z * (1.0f - t) + ed.z * t,

	};


}