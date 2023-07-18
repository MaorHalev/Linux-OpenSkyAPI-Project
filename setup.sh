#!/bin/bash

# Bash script to initialize the Flight Scanner Microservices setup
# Define variables for the image names and tags
DB_IMAGE="dbservice_image"
FLIGHTS_IMAGE="flightservice_image"
DB_TAG="v1.0"
FLIGHTS_TAG="v1.0"
DOCKER_HUB_USERNAME="doredelman1996"

# Function to check if Docker is installed
check_docker_installed() {
    if ! command -v docker &> /dev/null; then
        echo "Docker is not installed. Please install Docker and try again."
        exit 1
    fi
}

# Function to create a named pipe directory if it doesn't exist
create_named_pipe_directory() {
    if [ ! -d "/tmp/flights_pipe" ]; then
        mkdir -p /tmp/flights_pipe
    fi
}

# Function to pull Docker images from Docker Hub
pull_images() {
    echo "Pulling the dbService image..."
    docker pull $DOCKER_HUB_USERNAME/$DB_IMAGE:$DB_TAG

    echo "Pulling the FlightsService image..."
    docker pull $DOCKER_HUB_USERNAME/$FLIGHTS_IMAGE:$FLIGHTS_TAG
}

# Function to run the dbService container in the background
run_dbservice_container() {
    echo "Starting the dbService container..."
    docker run -d --name dbService --mount type=bind,source=/tmp/flights_pipe,target=/tmp/flights_pipe $DB_IMAGE:$DB_TAG
}

# Function to run the FlightsService container interactively
run_flightservice_container() {
    echo "Starting the FlightsService container..."
    docker run -it --name FlightsService --mount type=bind,source=/tmp/flights_pipe,target=/tmp/flights_pipe $FLIGHTS_IMAGE:$FLIGHTS_TAG
}

# Main function to run the script
main() {
    check_docker_installed
    create_named_pipe_directory
    pull_images
    run_dbservice_container
    run_flightservice_container
}

# Run the main function
main
