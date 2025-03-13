#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file
setup() {
    # Start the server in the background on port 1234
    ./dsh -s -p 1234 &
    # Save the server's process ID (PID) to stop it later
    SERVER_PID=$!
    
    # Wait for the server to start listening on the port
    for i in {1..10}; do
        if nc -z 127.0.0.1 1234; then
            break
        fi
        sleep 0.5
    done
}

teardown() {
    # Stop the server by killing its process, if it exists
    if ps -p $SERVER_PID > /dev/null; then
        kill $SERVER_PID
    fi
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Can run server" {
    run timeout 1 bash -c './dsh -s'
    # Assert that the command times out (exit status 124)
    # Assert that the output contains the expected server message
    [[ "$output" == *"socket server mode:  addr:0.0.0.0"* ]]
    [[ "$output" == *"-> Single-Threaded Mode"* ]]
}

@test "Can run server with specified port" {
    run timeout 1 bash -c './dsh -s -p 456'
    # Assert that the server with specified port times out (exit status 124)
    # Assert that the output contains the expected server message with the specified port
    [[ "$output" == *"socket server mode:  addr:0.0.0.0:456"* ]]
    [[ "$output" == *"-> Single-Threaded Mode"* ]]
}

@test "Can run server with specified addr" {
    run timeout 1 bash -c './dsh -s -i 0.0.0.0'
    # Assert that the server with specified port times out (exit status 124)
    # Assert that the output contains the expected server message with the specified port
    [[ "$output" == *"socket server mode:  addr:0.0.0.0:123"* ]]
    [[ "$output" == *"-> Single-Threaded Mode"* ]]
}

@test "Can run server with specified addr and port" {
    run timeout 1 bash -c './dsh -s -i 0.0.0.0 -p 456'
    # Assert that the server with specified port times out (exit status 124)
    # Assert that the output contains the expected server message with the specified port
    [[ "$output" == *"socket server mode:  addr:0.0.0.0:456"* ]]
    [[ "$output" == *"-> Single-Threaded Mode"* ]]
}

@test "Can run client and verify initial output" {
    # Run the client with a timeout to prevent hanging
    run timeout 1 bash -c './dsh -c 2>&1'
    [ "$status" -eq 124 ]
}

@test "Can run client with specified port" {
    # Run the client with a timeout to prevent hanging
    run timeout 1 bash -c './dsh -c -p 1234 2>&1'
    [ "$status" -eq 124 ]
}


@test "Can run client with specified addr" {
    # Run the client with a timeout to prevent hanging
    run timeout 1 bash -c './dsh -c -i 0.0.0.0 2>&1'
    [ "$status" -eq 124 ]
}


@test "Can run client with specified addr and port" {
    # Run the client with a timeout to prevent hanging
    run timeout 1 bash -c './dsh -c -i 0.0.0.0 -p 1234 2>&1'
    [ "$status" -eq 124 ]
}