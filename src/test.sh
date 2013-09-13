assert() {
    if [ $? -ne 0 ]
    then
        echo FAILED 1>&2
    fi
}

not_assert() {
    if [ $? -eq 0 ]
    then
        echo FAILED 1>&2
    fi
}

echo ''            | ./sgrep '' > /dev/null; assert
echo 'a'           | ./sgrep '' > /dev/null; not_assert

echo ''            | ./sgrep 'a' > /dev/null; not_assert
echo 'a'           | ./sgrep 'a' > /dev/null; assert
echo 'abc'         | ./sgrep 'abc' > /dev/null; assert

echo ''            | ./sgrep 'a*' > /dev/null; assert
echo 'a'           | ./sgrep 'a*' > /dev/null; assert
echo 'aa'          | ./sgrep 'a*' > /dev/null; assert
echo 'aaa'         | ./sgrep 'a*' > /dev/null; assert
echo 'ababab'      | ./sgrep '(ab)*' > /dev/null; assert
echo 'ababa'       | ./sgrep '(ab)*' > /dev/null; not_assert

echo ''            | ./sgrep 'a+' > /dev/null; not_assert
echo 'a'           | ./sgrep 'a+' > /dev/null; assert
echo 'aa'          | ./sgrep 'a+' > /dev/null; assert
echo 'aaa'         | ./sgrep 'a+' > /dev/null; assert
echo 'ababab'      | ./sgrep '(ab)+' > /dev/null; assert

echo 'a'           | ./sgrep 'a|b' > /dev/null; assert
echo 'b'           | ./sgrep '(a|b)' > /dev/null; assert
echo 'a'           | ./sgrep 'a|b|c' > /dev/null; assert
echo 'b'           | ./sgrep 'a|b|c' > /dev/null; assert
echo 'c'           | ./sgrep '(a|b|c)' > /dev/null; assert
echo 'd'           | ./sgrep 'a|b|c' > /dev/null; not_assert
echo ''            | ./sgrep '(a|b|c)' > /dev/null; not_assert
echo 'ab'          | ./sgrep '(ab|cd)' > /dev/null; assert
echo 'ad'          | ./sgrep '(ab|cd)' > /dev/null; not_assert
# echo 'ac'          | ./sgrep '(a|b)|(c|d)' > /dev/null; assert
# echo 'ad'          | ./sgrep '(a|b)|(c|d)' > /dev/null; assert
# echo 'bc'          | ./sgrep '(a|b)|(c|d)' > /dev/null; assert
# echo 'bd'          | ./sgrep '(a|b)|(c|d)' > /dev/null; assert

echo 'aaacddd'     | ./sgrep 'a*(b|c)d+' > /dev/null; assert
echo 'cd'          | ./sgrep 'a*(b|c)d+' > /dev/null; assert
echo 'aaaac'       | ./sgrep 'a*(b|c)d+' > /dev/null; not_assert
