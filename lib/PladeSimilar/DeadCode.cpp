// C-Style readFile
void readFile(const char* fileName, char** ret, long* size) {
  auto f = fopen(PladeHelper::Locate::UTF8ToLocate(fileName), "rb");
  fseek(f, 0, SEEK_END);
  *size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *ret = static_cast<char*>(malloc(*size + 1));
  fread(*ret, *size, 1, f);
  (*ret)[*size] = '\0';
  fclose(f);
}

// Time: O(m*n), Space: O(m*n)
size_t Levenshtein(const char* text1, const size_t len11, const char* text2, const size_t len22) {
  auto len1 = len11 + 1, len2 = len22 + 1;
  auto d = new size_t[len1 * len2];
  memset(d, 0, sizeof(size_t) * len1 * len2);
  for (size_t i = 1, im = 0; i < len2; ++i, ++im) {
    for (size_t j = 1, jn = 0; j < len1; ++j, ++jn) {
      if (text1[jn] == text2[im]) {
        d[(i * len1) + j] = d[((i - 1) * len1) + (j - 1)];
      }
      else {
        d[(i * len1) + j] = min(d[(i - 1) * len1 + j] + 1, min(d[i * len1 + (j - 1)] + 1, d[(i - 1) * len1 + (j - 1)] + 1));
      }
    }
  }
  auto r = d[len11 * len22 - 1];
  delete[] d;
  return r;
}

// Time: O(n^3)
long SimilarText(const char* text1, const size_t len1, const char* text2, const size_t len2) {
  long l, pos1 = 0, pos2 = 0, max = 0;
  for (auto p = 0; p < len1; p++) {
    for (auto q = 0; q < len2; q++) {
      for (l = 0; p + l < len1 && q + l < len2 && text1[p + l] == text2[q + l]; l++) {}
      if (l > max) {
        max = l;
        pos1 = p;
        pos2 = q;
      }
    }
  }
  auto sum = max;
  if (sum) {
    if (pos1 && pos2) {
      sum += SimilarText(text1, pos1, text2, pos2);
    }
    if (pos1 + max < len1 && pos2 + max < len2) {
      sum += SimilarText(text1 + pos1 + max, len1 - pos1 - max, text2 + pos2 + max, len2 - pos2 - max);
    }
  }
  return sum;
}
