echo Running TEST 1

cat test.1 > output1
./my-cat test.1 > output2

TEST_1_FAILED_MSG="TEST 1: cat test.1 != ./my-cat test.1"
TEST_2_FAILED_MSG="TEST 2: cat test.1 test.2 test.3 != ./my-cat test.1 test.2 test.3"
TEST_3_FAILED_MSG="TEST 3: ./my-cat != 0"
TEST_4_FAILED_MSG="TEST 4: ./my-cat test.1 test.2 test.4 != 1"
TEST_1_FAILED=0
TEST_2_FAILED=0
TEST_3_FAILED=0
TEST_4_FAILED=0
if ! cmp output1 output2 
then
  echo $TEST_1_FAILED_MSG
  exit 1
fi

echo Running TEST 2

rm output1
rm output2

cat test.1 test.2 test.3 > output1
./my-cat test.1 test.2 test.3 > output2

if ! cmp output1 output2
then
  echo $TEST_2_FAILED_MSG
  exit 1
fi

echo Running TEST 3

./my-cat

if [ $? -ne 0 ]
then
  echo $TEST_3_FAILED_MSG
  exit 1
fi

echo Running TEST 4

./my-cat test.1 test.2 test.4

if [ $? -ne 1 ]
then
  echo $TEST_4_FAILED_MSG
  exit 1
fi

echo "-------------"
echo "Final Results:"
echo TEST 1: Passed
echo TEST 2: Passed
echo TEST 3: Passed
echo TEST 4: Passed

rm output1
rm output2

exit 0
