#include "Twist.h"

namespace Twist {
	ThemeFloatOp::ThemeFloatOp(int sourceId_) : sourceId(sourceId_) { }

	FloatOp ThemeFloatOp::operator+(float value) {
		int localSourceId = sourceId;
		return [=]() {
			float original = Theme::getFloat(localSourceId);
			return original + value;
		};
	}

	FloatOp ThemeFloatOp::operator-(float value) {
		int localSourceId = sourceId;
		return [=]() {
			float original = Theme::getFloat(localSourceId);
			return original - value;
		};
	}

	FloatOp ThemeFloatOp::operator*(float value) {
		int localSourceId = sourceId;
		return [=]() {
			float original = Theme::getFloat(localSourceId);
			return original * value;
		};
	}

	FloatOp ThemeFloatOp::operator/(float value) {
		int localSourceId = sourceId;
		return [=]() {
			float original = Theme::getFloat(localSourceId);
			return original / value;
		};
	}

	ValueFloatOp::ValueFloatOp(float value_) : value(value_) { }

	FloatOp ValueFloatOp::operator+(float other) {
		float localValue = value;
		return [=]() {
			return localValue + value;
		};
	}

	FloatOp ValueFloatOp::operator-(float other) {
		float localValue = value;
		return [=]() {
			return localValue - value;
		};
	}

	FloatOp ValueFloatOp::operator*(float other) {
		float localValue = value;
		return [=]() {
			return localValue * value;
		};
	}

	FloatOp ValueFloatOp::operator/(float other) {
		float localValue = value;
		return [=]() {
			return localValue / value;
		};
	}

	namespace Theme {
		static std::map<int, float> valuedFloats;
		static std::map<int, FloatOp> calculatedFloats;

		void setFloat(int id, FloatOp op) {
			calculatedFloats[id] = op;
		}

		void setFloat(int id, float value) {
			valuedFloats[id] = value;
		}

		float getFloat(int id) {
			auto valueIter = valuedFloats.find(id);
			if (valueIter != valuedFloats.end()) {
				return valueIter->second;
			}

			auto calculateIter = calculatedFloats.find(id);
			if (calculateIter != calculatedFloats.end()) {
				return calculateIter->second();
			}

			throw std::domain_error("Twist: Requesting nonexistent Theme property.");
		}

		ThemeFloatOp floatSource(int id) {
			return ThemeFloatOp(id);
		}
	}
}