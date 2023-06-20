flags=("-b" "-n" "-s" "-t" "-e" "-v")

cat_test() {
    "./s21_cat" $1 "test_case.txt" >> s21.txt
    "cat" $1 "test_case.txt" >> sys.txt
    res_diff="$(diff -s s21.txt sys.txt)"
    if [ "$res_diff" == "Files s21.txt and sys.txt are identical" ]
    then
        echo "Flags $1 test: SUCCESS"
    else
        echo "Flags $1 test: FAIL"
    fi
    rm -f s21.txt sys.txt
}

echo "#####################"
echo "## TESTING S21_CAT ##"
echo "#####################"

for flag1 in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        for flag3 in "${flags[@]}"
        do
            if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] \
            && [ $flag1 != $flag3 ] 
            then
                flag_comb="$flag1 $flag2 $flag3"
                cat_test "$flag_comb"
            fi
        done
    done
done