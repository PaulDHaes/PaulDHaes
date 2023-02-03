# Use an existing image as the base image
FROM alpine

# Set the working directory in the container
WORKDIR /app

# Copy files from your host to the container
COPY index.html .

# Run a command to install dependencies
RUN apk add --no-cache nginx

# Set the command that will run when the container starts
CMD ["nginx", "-g", "daemon off;"]

# Expose a port for the nginx server
EXPOSE 80
