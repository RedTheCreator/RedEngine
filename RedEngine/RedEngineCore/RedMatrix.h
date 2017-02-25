#pragma once

#include "RedCommon.h"

namespace Red
{
	struct RMatrix4x4
	{
		static RMatrix4x4 IDENTITY;
		RFloat m[4][4];

		RMatrix4x4()
		{
			memset(m, 0, sizeof(RFloat) * 16);
		}

		RMatrix4x4(const RMatrix4x4& right)
		{
			m[0][0] = right.m[0][0], m[0][1] = right.m[0][1], m[0][2] = right.m[0][2], m[0][3] = right.m[0][3];
			m[1][0] = right.m[1][0], m[1][1] = right.m[1][1], m[1][2] = right.m[1][2], m[1][3] = right.m[1][3];
			m[2][0] = right.m[2][0], m[2][1] = right.m[2][1], m[2][2] = right.m[2][2], m[2][3] = right.m[2][3];
			m[3][0] = right.m[3][0], m[3][1] = right.m[3][1], m[3][2] = right.m[3][2], m[3][3] = right.m[3][3];
		}

		RMatrix4x4(RFloat m0_0, RFloat m0_1, RFloat m0_2, RFloat m0_3,
			RFloat m1_0, RFloat m1_1, RFloat m1_2, RFloat m1_3,
			RFloat m2_0, RFloat m2_1, RFloat m2_2, RFloat m2_3,
			RFloat m3_0, RFloat m3_1, RFloat m3_2, RFloat m3_3)
		{
			m[0][0] = m0_0, m[0][1] = m0_1, m[0][2] = m0_2, m[0][3] = m0_3;
			m[1][0] = m1_0, m[1][1] = m1_1, m[1][2] = m1_2, m[1][3] = m1_3;
			m[2][0] = m2_0, m[2][1] = m2_1, m[2][2] = m2_2, m[2][3] = m2_3;
			m[3][0] = m3_0, m[3][1] = m3_1, m[3][2] = m3_2, m[3][3] = m3_3;
		}

		inline RMatrix4x4 transpose() const
		{
			RMatrix4x4 temp;
			for (RInt i = 0; i < 4; i++)
			{
				for (RInt j = 0; j < 4; j++)
				{
					temp.m[i][j] = m[j][i];
				}
			}
			return temp;
		}

		inline RFloat at(size_t i, size_t j) const
		{
			return m[i][j];
		}

		inline RFloat& at(size_t i, size_t j)
		{
			return m[i][j];
		}

		inline RMatrix4x4 operator + (const RMatrix4x4& right) const
		{
			return RMatrix4x4(
				m[0][0] + right.m[0][0], m[0][1] + right.m[0][1], m[0][2] + right.m[0][2], m[0][3] + right.m[0][3],
				m[1][0] + right.m[1][0], m[1][1] + right.m[1][1], m[1][2] + right.m[1][2], m[1][3] + right.m[1][3],
				m[2][0] + right.m[2][0], m[2][1] + right.m[2][1], m[2][2] + right.m[2][2], m[2][3] + right.m[2][3],
				m[3][0] + right.m[3][0], m[3][1] + right.m[3][1], m[3][2] + right.m[3][2], m[3][3] + right.m[3][3]);
		}

		inline RMatrix4x4 operator - (const RMatrix4x4& right) const
		{
			return RMatrix4x4(
				m[0][0] - right.m[0][0], m[0][1] - right.m[0][1], m[0][2] - right.m[0][2], m[0][3] - right.m[0][3],
				m[1][0] - right.m[1][0], m[1][1] - right.m[1][1], m[1][2] - right.m[1][2], m[1][3] - right.m[1][3],
				m[2][0] - right.m[2][0], m[2][1] - right.m[2][1], m[2][2] - right.m[2][2], m[2][3] - right.m[2][3],
				m[3][0] - right.m[3][0], m[3][1] - right.m[3][1], m[3][2] - right.m[3][2], m[3][3] - right.m[3][3]);
		}

		inline RMatrix4x4 operator * (RFloat factor) const
		{
			return RMatrix4x4(
				m[0][0] * factor, m[0][1] * factor, m[0][2] * factor, m[0][3] * factor,
				m[1][0] * factor, m[1][1] * factor, m[1][2] * factor, m[1][3] * factor,
				m[2][0] * factor, m[2][1] * factor, m[2][2] * factor, m[2][3] * factor,
				m[3][0] * factor, m[3][1] * factor, m[3][2] * factor, m[3][3] * factor);
		}

		inline RMatrix4x4 operator * (const RMatrix4x4& right) const
		{
			RMatrix4x4 temp;
			for (RInt i = 0; i < 4; i++)
			{
				for (RInt j = 0; j < 4; j++)
				{
					for (RInt k = 0; k < 4; k++)
					{
						temp.m[i][j] += m[i][k] * right.m[k][j];
					}
				}
			}
			return temp;
		}

		inline RMatrix4x4& operator = (const RMatrix4x4& right)
		{
			m[0][0] = right.m[0][0], m[0][1] = right.m[0][1], m[0][2] = right.m[0][2], m[0][3] = right.m[0][3];
			m[1][0] = right.m[1][0], m[1][1] = right.m[1][1], m[1][2] = right.m[1][2], m[1][3] = right.m[1][3];
			m[2][0] = right.m[2][0], m[2][1] = right.m[2][1], m[2][2] = right.m[2][2], m[2][3] = right.m[2][3];
			m[3][0] = right.m[3][0], m[3][1] = right.m[3][1], m[3][2] = right.m[3][2], m[3][3] = right.m[3][3];
		}

		inline bool operator == (const RMatrix4x4& right) const
		{
			if (m[0][0] != right.m[0][0] || m[0][1] != right.m[0][1] || m[0][2] != right.m[0][2] || m[0][3] != right.m[0][3] ||
				m[1][0] != right.m[1][0] || m[1][1] != right.m[1][1] || m[1][2] != right.m[1][2] || m[1][3] != right.m[1][3] ||
				m[2][0] != right.m[2][0] || m[2][1] != right.m[2][1] || m[2][2] != right.m[2][2] || m[2][3] != right.m[2][3] ||
				m[3][0] != right.m[3][0] || m[3][1] != right.m[3][1] || m[3][2] != right.m[3][2] || m[3][3] != right.m[3][3])
				return false;

			return true;
		}
	};
}