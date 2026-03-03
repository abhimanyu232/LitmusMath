#include <gtest/gtest.h>
#include "common.h"
#include "../libs/serial/statistics.h"

class StatisticsTest : public testing::Test {
 protected:
  StatisticsTest() = default;

  std::vector<int> int_vec_1{10, 2, 38, 23, 38, 23, 21, 23};
  // {mean, median, variance, sample_variance, geo_mean}
  std::vector<float_type> ivec1_stats_actual{22.25, 23, 132.4375,
											 151.35714285714, 17.119851726053};

  std::vector<int> int_vec_2{
	  7764, 9793, 7939, 5489, 3941, 5032, 3676, 9815, 3172, 4055,
	  4439, 6948, 2050, 9924, 4958, 6865, 2958, 1087, 2614, 1802,
	  4107, 1984, 3080, 8690, 4685, 2882, 5222, 8187, 3179, 4642,
	  7012, 4239, 7612, 7050, 6395, 1656, 8,	8604, 7355, 3474,
	  8314, 5151, 9199, 7644, 1034, 5764, 8049, 5541, 282};
  // {mean, median, variance, sample)variance, geo_mean}
  std::vector<float_type> ivec2_stats_actual{
	  5211.4693877551, 5032, 7088100.6980425, 7235769.462585, 3923.4059822675};

  std::vector<float_type> float_vec_1{1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8};
  // {mean, median, variance, sample)variance, geo_mean}
  std::vector<float_type> fvec1_stats_actual{4.5, 4.5, 4.84, 5.6466666666667,
											 3.8420713057318};

  std::vector<float_type> float_vec_2{
	  56.6875, 111.1771, 67.6534, 88.7396, 11.1050,	 107.3175,
	  51.2599, 18.6375,	 13.0012, 90.7213, 105.7054, 3.0538,
	  6.1273,  29.9811,	 95.6204, 33.6322, 45.3466,	 21.6981,
	  3.3258,  101.1523, 26.9900, 18.0077, 17.7181,	 31.3482,
	  46.7193, 99.9779,	 45.5010, 33.4367, 87.9289,	 37.5083};
  // {mean, median, variance, sample)variance, geo_mean}
  std::vector<float_type> fvec2_stats_actual{
	  50.23597, 41.42745, 1244.9569179651, 1287.8864668605, 34.960556722017};

  Statistics int_vector_stats;
  Statistics float_vector_stats;
};

TEST_F(StatisticsTest, IntegerDataTest) {
  int_vector_stats.UpdateSummaryStats<int>(int_vec_1);
  // int_vector_stats.PrintSummaryStats();
  EXPECT_TRUE(std::fabs(int_vector_stats.mean - ivec1_stats_actual[0]) <= 1e-6);
  EXPECT_TRUE(std::fabs(int_vector_stats.median - ivec1_stats_actual[1]) <=
			  1e-6);
  EXPECT_TRUE(std::fabs(int_vector_stats.variance - ivec1_stats_actual[2]) <=
			  1e-2);
  // todo: fix variance calculation, tighten test bounds
  EXPECT_TRUE(
	  std::fabs(int_vector_stats.sample_variance - ivec1_stats_actual[3]) <= 1);
  EXPECT_TRUE(std::fabs(int_vector_stats.geometric_mean -
						ivec1_stats_actual[4]) <= 1e-2);

  int_vector_stats.ResetSummaryStats();

  int_vector_stats.UpdateSummaryStats<int>(int_vec_2);
  // int_vector_stats.PrintSummaryStats();
  EXPECT_TRUE(std::fabs(int_vector_stats.mean - ivec2_stats_actual[0]) <= 1e-6);
  EXPECT_TRUE(std::fabs(int_vector_stats.median - ivec2_stats_actual[1]) <=
			  1e-6);
  EXPECT_TRUE(std::fabs(int_vector_stats.variance - ivec2_stats_actual[2]) <=
			  1e-2);
  // todo: fix variance calculation, tighten test bounds
  EXPECT_TRUE(
	  std::fabs(int_vector_stats.sample_variance - ivec2_stats_actual[3]) <= 1);
  EXPECT_TRUE(std::fabs(int_vector_stats.geometric_mean -
						ivec2_stats_actual[4]) <= 1e-2);
}

TEST_F(StatisticsTest, FloatDataTest) {

  float_vector_stats.UpdateSummaryStats<float_type>(float_vec_1);
  // float_vector_stats.PrintSummaryStats();

  EXPECT_TRUE(std::fabs(float_vector_stats.mean - fvec1_stats_actual[0]) <=
			  1e-6);
  EXPECT_TRUE(std::fabs(float_vector_stats.median - fvec1_stats_actual[1]) <=
			  1e-6);

  EXPECT_TRUE(std::fabs(float_vector_stats.variance - fvec1_stats_actual[2]) <=
			  1e-2);
  // todo: fix variance calculation, tighten test bounds
  EXPECT_TRUE(std::fabs(float_vector_stats.sample_variance -
						fvec1_stats_actual[3]) <= 1);

  EXPECT_TRUE(std::fabs(float_vector_stats.geometric_mean -
						fvec1_stats_actual[4]) <= 1e-2);

  float_vector_stats.ResetSummaryStats();

  float_vector_stats.UpdateSummaryStats<float_type>(float_vec_2);
  // float_vector_stats.PrintSummaryStats();

  EXPECT_TRUE(std::fabs(float_vector_stats.mean - fvec2_stats_actual[0]) <=
			  1e-6);
  EXPECT_TRUE(std::fabs(float_vector_stats.median - fvec2_stats_actual[1]) <=
			  1e-6);

  EXPECT_TRUE(std::fabs(float_vector_stats.variance - fvec2_stats_actual[2]) <=
			  1e-2);
  // todo: fix variance calculation, tighten test bounds
  EXPECT_TRUE(std::fabs(float_vector_stats.sample_variance -
						fvec2_stats_actual[3]) <= 1);

  EXPECT_TRUE(std::fabs(float_vector_stats.geometric_mean -
						fvec2_stats_actual[4]) <= 1e-2);
}