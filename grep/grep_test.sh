flags=("-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o")


grep_test() {
    "./s21_grep" $1 "test_case.txt" >> s21.txt
    "grep" $1 "test_case.txt" >> sys.txt
    res_diff="$(diff -s s21.txt sys.txt)"
    if [ "$res_diff" == "Files s21.txt and sys.txt are identical" ]
    then
        echo "Flags |$1| test: SUCCESS"
    else
        echo "Flags |$1| test: FAIL"
    fi
    rm -f s21.txt sys.txt
}

echo "#####################"
echo "## TESTING S21_GREP ##"
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
                flag_comb="$flag1 $flag2 $flag3 -f test_patt.txt"
                grep_test "$flag_comb"
            fi
        done
    done
done