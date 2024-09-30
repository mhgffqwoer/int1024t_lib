#include "number.h"

static const size_t kByteLength = 0x8;
static const size_t kFirstByteMask = 0xFF;
static const int2023_t kOne = from_int(1);

int2023_t from_int(int32_t value) {
  int64_t temp_value = value;
  bool is_negative = (value < 0);
  int2023_t result;

  if (is_negative) {
    temp_value = (~temp_value + 1);
  }  

  for (int i = result.kSize - 1; i >= 0; --i) {
    result.value[i] = temp_value & kFirstByteMask;
    temp_value >>= kByteLength;
    if (value == 0) break;
  }

  if (is_negative) !result;

  return result;
}

int2023_t from_string(const char* buff) {
  const int size = strlen(buff);
  char temp[size];
  strcpy(temp, buff);
  int2023_t result;

  bool is_negative = (buff[0] == '-');

  for (int i = result.kSize - 1; i >= 0; --i) {
    uint8_t cnt = 0;
    for (int j = 0; j < kByteLength; ++j) {
      int remainder = 0;
      for (int k = is_negative; k < size; ++k) {
        int number = temp[k] - 48;
        temp[k] = ((remainder * 10 + number) >> 1) + 48;
        remainder = (remainder * 10 + number) & 1;
      }
      cnt += (remainder * (1 << j));
    }
    result.value[i] = cnt;
  }

  if (is_negative) !result;

  return result;
}

void operator<<=(int2023_t& value, const int& shift) {
  int buff = 0;

  for (int i = value.kSize - 1; i >= 0; --i) {
    int temp = value.value[i] << shift;
    value.value[i] = (temp & kFirstByteMask) + buff;
    buff = temp >> kByteLength;
  }
}

void operator!(int2023_t& value) {
  for (int i = value.kSize - 1; i >= 0; --i) {
    value.value[i] = ~value.value[i];
  }

  value = value + kOne;//from_int(1);
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;

  int j = 0;
  for (int i = result.kSize - 1; i >= 0; --i) {
    int temp = (lhs.value[i] + rhs.value[i] + j);
    result.value[i] = (temp & kFirstByteMask);
    j = (temp >> kByteLength);
  }

  return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = rhs;
  !result;
  return (result + lhs);
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t lhs_copy = lhs;
  int2023_t result;

  for (int i = result.kSize - 1; i >= 0; --i) {
    int temp = rhs.value[i];
    for (int j = 0; j < kByteLength; ++j) {
      if ((temp & 1) == 1) {
        result = result + lhs_copy;
      }
      temp >>= 1;
      lhs_copy <<= 1;
    }
  }
  return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;
  int2023_t buff;
  int2023_t lhs_copy = lhs;
  int2023_t rhs_copy = rhs;

  bool is_negative_lhs = (lhs.value[0] >= 128);
  bool is_negative_rhs = (rhs.value[0] >= 128);
  bool is_negative_result = (is_negative_lhs ^ is_negative_rhs);

  if (is_negative_lhs) !lhs_copy;
  if (is_negative_rhs) !rhs_copy;

  for (int i = 0; i < result.kSize; ++i) {
    for (int j = kByteLength - 1; j >= 0; --j) {
      buff <<= 1;
      result <<= 1;
      if (lhs_copy.value[i] >> j) {
        buff = buff + kOne;//from_int(1);
      }
      lhs_copy.value[i] = lhs_copy.value[i] & ((1 << j) - 1);
      if ((buff > rhs_copy) || (buff == rhs_copy)) {
        result = result + kOne;//from_int(1);
        buff = buff - rhs_copy;
      }
    }
  }

  if (is_negative_result) !result;

  return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i < lhs.kSize; ++i) {
    if (lhs.value[i] ^ rhs.value[i]) return false;
  }

  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs == rhs);
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
  bool is_negative_lhs = (lhs.value[0] >= 128);
  bool is_negative_rhs = (rhs.value[0] >= 128);
  if (is_negative_lhs != is_negative_rhs)
    return !(is_negative_lhs > is_negative_rhs);

  if (!is_negative_lhs)
    for (int i = 0; i < lhs.kSize; ++i) {
      if (lhs.value[i] > rhs.value[i]) return true;
      if (lhs.value[i] < rhs.value[i]) return false;
    }
  else
    for (int i = 0; i < lhs.kSize; ++i) {
      if (lhs.value[i] > rhs.value[i]) return true;
      if (lhs.value[i] < rhs.value[i]) return false;
    }

  return false;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  bool is_negative_lhs = (lhs.value[0] >= 128);
  bool is_negative_rhs = (rhs.value[0] >= 128);
  if (is_negative_lhs != is_negative_rhs)
    return !(is_negative_lhs < is_negative_rhs);

  if (!is_negative_lhs)
    for (int i = 0; i < lhs.kSize; ++i) {
      if (lhs.value[i] < rhs.value[i]) return true;
      if (lhs.value[i] > rhs.value[i]) return false;
    }
  else
    for (int i = 0; i < lhs.kSize; ++i) {
      if (lhs.value[i] < rhs.value[i]) return true;
      if (lhs.value[i] > rhs.value[i]) return false;
    }

  return false;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  int2023_t temp_value = value;

  if (value.value[0] >= 128) {
    stream << "-";
    !temp_value;
  }

  bool flag = false;
  for (int i = 0; i < value.kSize; ++i) {
    for (int j = kByteLength - 1; j >= 0; --j) {
      int bit = temp_value.value[i] | (1 << j);
      temp_value.value[i] &= (1 << j) - 1;
      if (bit) {
        stream << bit;
        flag = true;
      } else {
        if (flag) stream << bit;
      }
    }
  }

  return stream;
}
