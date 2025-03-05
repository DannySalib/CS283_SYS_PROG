#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Shell runs" {
    run ./dsh <<EOF
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echo hello world" {
    run ./dsh <<EOF
echo hello world
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="helloworlddsh2>dsh2>cmdloopreturned0"


    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "uname runs with no errors" {
    run ./dsh <<EOF
uname -a
EOF

    [ "$status" -eq 0 ]
}

@test "pwd runs with no errors" {
    run ./dsh <<EOF
pwd
EOF

    [ "$status" -eq 0 ]
}

@test "Handles quotes " {
    run ./dsh <<EOF
echo "first" middle "last"
EOF
    
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="firstmiddlelastdsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Handles single and double quotes" {
    run ./dsh <<EOF
echo 'first' "  middle  " last
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="first   middle   lastdsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "runs when command has incomplete quotes" {
    run ./dsh <<EOF
echo "hello
echo 'hello
echo hello"
echo hello'
EOF
    [ "$status" -eq 0 ]
}

@test "Command with trailing spaces" {
    run ./dsh <<EOF
echo         hello       
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hellodsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Handles quotes with \\ symbol in it" {
    run ./dsh <<EOF
echo "a\tb"
EOF
    
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="a\tbdsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Handles one pipeline command" {
    echo -e "a\na\na" > tmpfile
    run "./dsh" <<EOF                
cat tmpfile | grep a | wc -l
EOF
    rm tmpfile

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="3dsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "No issues when running a command many times" {
    # Number of times to run the command
    num_runs=100

    # Generate input with 100 "echo Hello" commands
    input=$(for i in $(seq 1 $num_runs); do echo "echo Hello"; done)

    # Run the commands in dsh
    run ./dsh <<EOF
$input
EOF

    # Expected output: 100 "Hello"s and 100 "dsh2>" prompts
    expected_output=$(printf 'Hello%.0s' $(seq 1 $num_runs))
    expected_output+=$(printf 'dsh2>%.0s' $(seq 1 $num_runs))
    expected_output+="dsh2>"
    expected_output+="cmdloopreturned0"

    # Strip all whitespace and remove "cmdloopreturned0" from the captured output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    stripped_expected=$(echo "$expected_output" | tr -d '[:space:]')

    # Debugging output (only shown on failure)
    echo "--- Captured Output ---"
    echo "$stripped_output"
    echo "--- Expected Output ---"
    echo "$stripped_expected"

    # Assert equality
    [ "$stripped_output" = "$stripped_expected" ]
}

@test "No issues when running a QUOTED command many times" {
    # Number of times to run the command
    num_runs=100

    # Generate input with 100 "echo Hello" commands
    input=$(for i in $(seq 1 $num_runs); do echo "echo 'Hello'"; done)

    # Run the commands in dsh
    run ./dsh <<EOF
$input
EOF

    # Expected output: 100 "Hello"s and 100 "dsh2>" prompts
    expected_output=$(printf 'Hello%.0s' $(seq 1 $num_runs))
    expected_output+=$(printf 'dsh2>%.0s' $(seq 1 $num_runs))
    expected_output+="dsh2>"
    expected_output+="cmdloopreturned0"

    # Strip all whitespace and remove "cmdloopreturned0" from the captured output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    stripped_expected=$(echo "$expected_output" | tr -d '[:space:]')

    # Debugging output (only shown on failure)
    echo "--- Captured Output ---"
    echo "$stripped_output"
    echo "--- Expected Output ---"
    echo "$stripped_expected"

    # Assert equality
    [ "$stripped_output" = "$stripped_expected" ]
}

@test "No issues when running a pipeline command many times" {
    # Number of times to run the command
    num_runs=100

    # Generate input with 100 "echo Hello" commands
    input=$(for i in $(seq 1 $num_runs); do echo "cat tmpfile | grep a | wc -l"; done)

    # Run the commands in dsh
        echo -e "a\na\na" > tmpfile
    run "./dsh" <<EOF                
$input
EOF
    rm tmpfile

    # Expected output: 100 "Hello"s and 100 "dsh2>" prompts
    expected_output=$(printf '3%.0s' $(seq 1 $num_runs))
    expected_output+=$(printf 'dsh2>%.0s' $(seq 1 $num_runs))
    expected_output+="dsh2>"
    expected_output+="cmdloopreturned0"

    # Strip all whitespace and remove "cmdloopreturned0" from the captured output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    stripped_expected=$(echo "$expected_output" | tr -d '[:space:]')

    # Debugging output (only shown on failure)
    echo "--- Captured Output ---"
    echo "$stripped_output"
    echo "--- Expected Output ---"
    echo "$stripped_expected"

    # Assert equality
    [ "$stripped_output" = "$stripped_expected" ]
}

@test "No issues when running a pipeline WITH QUOTES command many times" {
    # Number of times to run the command
    num_runs=100

    # Generate input with 100 "echo Hello" commands
    input=$(for i in $(seq 1 $num_runs); do echo "cat tmpfile | grep 'a' | wc -l"; done)

    # Run the commands in dsh
        echo -e "a\na\na" > tmpfile
    run "./dsh" <<EOF                
$input
EOF
    rm tmpfile

    # Expected output: 100 "Hello"s and 100 "dsh2>" prompts
    expected_output=$(printf '3%.0s' $(seq 1 $num_runs))
    expected_output+=$(printf 'dsh2>%.0s' $(seq 1 $num_runs))
    expected_output+="dsh2>"
    expected_output+="cmdloopreturned0"

    # Strip all whitespace and remove "cmdloopreturned0" from the captured output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    stripped_expected=$(echo "$expected_output" | tr -d '[:space:]')

    # Debugging output (only shown on failure)
    echo "--- Captured Output ---"
    echo "$stripped_output"
    echo "--- Expected Output ---"
    echo "$stripped_expected"

    # Assert equality
    [ "$stripped_output" = "$stripped_expected" ]
}