# Use a base image with Qt 6 installed
FROM qtci/ubuntu-20.04-qt:6.2.2

# Set the working directory
WORKDIR /app

# Install additional dependencies (if needed)
# For example, you may need to install libraries required by your application

# Copy the CMakeLists.txt file into the container
COPY CMakeLists.txt .

# Copy the entire source code into the container
COPY . .

# Create a build directory and set it as the working directory
RUN mkdir build
WORKDIR /app/build

# Configure and build the application
RUN cmake ..
RUN make

# Set the entry point for the container
CMD ["./your_application_executable"]
