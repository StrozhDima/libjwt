/* Public domain, no copyright. Use at your own risk. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <check.h>

#include <jwt.h>

START_TEST(test_jwt_new)
{
	jwt_t *jwt = NULL;
	int ret = 0;

	ret = jwt_new(&jwt);
	ck_assert_int_eq(ret, 0);
	ck_assert(jwt != NULL);

	jwt_free(jwt);
}
END_TEST

START_TEST(test_jwt_dup)
{
	jwt_t *jwt = NULL, *new = NULL;
	int ret = 0;
	const char *val = NULL;

	ret = jwt_new(&jwt);
	ck_assert_int_eq(ret, 0);
	ck_assert(jwt != NULL);

	ret = jwt_add_grant(jwt, "iss", "test");
	ck_assert_int_eq(ret, 0);

	new = jwt_dup(jwt);
	ck_assert(new != NULL);

	val = jwt_get_grant(new, "iss");
	ck_assert(val != NULL);
	ck_assert_str_eq(val, "test");

	jwt_free(new);
	jwt_free(jwt);
}
END_TEST

START_TEST(test_jwt_decode)
{
	const char token[] = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzM4NCJ9."
			     "eyJpc3MiOiJmaWxlcy5jeXBocmUuY29tIiwic"
			     "3ViIjoidXNlcjAifQ==.";
	jwt_t *jwt;
	int ret;

	ret = jwt_decode(&jwt, token, NULL, 0);
	ck_assert_int_eq(ret, 0);
	ck_assert(jwt != NULL);

	jwt_free(jwt);
}
END_TEST

START_TEST(test_jwt_decode_hs256)
{
	const char token[] = "eyJhbGciOiJub25lIn0=.eyJpc3MiOiJmaWxl"
			     "cy5jeXBocmUuY29tIiwic3ViIjoidXNlcjAif"
			     "Q==.eyJhbGciOiJub25lIn0=.eyJpc3MiOiJm"
			     "aWxlcy5jeXBocmUuY29tIiwic3ViIjoidXNlc"
			     "jAifQ==";
	unsigned char key256[32] = "012345678901234567890123456789XY";
	jwt_t *jwt;
	int ret;

	ret = jwt_decode(&jwt, token, key256, sizeof(key256));
	ck_assert_int_eq(ret, 0);
	ck_assert(jwt != NULL);

	jwt_free(jwt);
}
END_TEST

START_TEST(test_jwt_decode_hs384)
{
	const char token[] = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzM4NCJ9."
			     "eyJpc3MiOiJmaWxlcy5jeXBocmUuY29tIiwic"
			     "3ViIjoidXNlcjAifQ==.k9MApCWNkjZi47zVz"
			     "Pw/SkHOPEtlMuzGcseuKqhzwfGaqnLp3aIArg"
			     "1wuUU+4QB2";
	const unsigned char key384[48] = "aaaabbbbccccddddeeeeffffg"
					 "ggghhhhiiiijjjjkkkkllll";
	jwt_t *jwt;
	int ret;

	ret = jwt_decode(&jwt, token, key384, sizeof(key384));
	ck_assert_int_eq(ret, 0);
	ck_assert(jwt != NULL);

	jwt_free(jwt);
}
END_TEST

Suite *libjwt_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("LibJWT New");

	tc_core = tcase_create("jwt_new");
	tcase_add_test(tc_core, test_jwt_new);
	tcase_add_test(tc_core, test_jwt_dup);
	tcase_add_test(tc_core, test_jwt_decode);
	tcase_add_test(tc_core, test_jwt_decode_hs256);
	tcase_add_test(tc_core, test_jwt_decode_hs384);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(int argc, char *argv[])
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = libjwt_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
