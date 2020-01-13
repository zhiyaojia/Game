#include <iostream>
#include <vector>
#include "Math.h"

struct BlockInfo
{
	Vector3 pos;
	float scale;
	float rotation;
	int index;
};


class BlockInfoStack
{
public:
	BlockInfoStack(){};

	bool empty()
	{
		return mArray.empty();
	}

	BlockInfo top()
	{
		BlockInfo b = mArray.back();
		mArray.pop_back();
		return b;
	}

	void push(Vector3 pos, float scale, int index, float rotation)
	{
		BlockInfo b;
		b.pos = pos;
		b.scale = scale;
		b.index = index;
		b.rotation = rotation;

		if(mArray.size() == mCapacity)
		{
			mArray.erase(mArray.begin());
		}
		mArray.emplace_back(b);
	}

private:
	int mCapacity = 5;
    std::vector<BlockInfo> mArray;
};


















