param (
    [string]$Option = 'Default'
)

function Log-Message {
    param (
        [string]$Message
    )
    Write-Host "$((Get-Date).ToString('yyyy-MM-dd HH:mm:ss')) - $Message"
}

function Invoke-Clean {
    Log-Message "Running clean option..."
    Invoke-DockerClean
    Invoke-KubernetesClean
}

function Invoke-DockerClean {
    Log-Message "Cleaning Docker resources..."
    docker-compose down --volumes
    docker volume prune -f
    docker image prune -a -f
}

function Invoke-KubernetesClean {
    Log-Message "Cleaning Kubernetes resources..."
    kubectl delete deployment db-deployment
    kubectl delete deployment backend-deployment
    kubectl delete deployment frontend-deployment
    kubectl delete service db-service
    kubectl delete service backend-service
    kubectl delete service frontend-service
}
function Invoke-DockerCleanBuildStart {
    Log-Message "Cleaning resources, building and starting Docker containers..."
    Invoke-Clean
    Invoke-DockerBuild
    Invoke-DockerStart
}

function Invoke-DockerStart {
    Log-Message "Starting Docker containers..."
    docker-compose up -d
}

function Invoke-DockerStop {
    Log-Message "Stopping Docker containers..."
    docker-compose stop
}

function Invoke-DockerRestart {
    Log-Message "Retarting Docker containers..."
    docker-compose stop
    docker-compose up -d
}

function Invoke-DockerBuild {
    Log-Message "Building Docker images..."
    docker-compose build
}

function Invoke-KubernetesCleanBuildStart {
    Log-Message "Cleaning resources, building Docker images, and starting Kubernetes resources..."
    Invoke-Clean
    Invoke-DockerBuild
    kubectl apply -f kubernetes/db-deployment.yaml
    kubectl apply -f kubernetes/backend-deployment.yaml
    kubectl apply -f kubernetes/frontend-deployment.yaml
    kubectl get deployments
    kubectl get pods
    kubectl get services
}

switch ($Option) {
    'Clean' { Invoke-Clean }
    'Docker' { Invoke-DockerCleanBuildStart }
    'Kubernetes' { Invoke-KubernetesCleanBuildStart }
    'DockerStart' { Invoke-DockerStart }
    'DockerStop' { Invoke-DockerStop }
    'DockerRestart' { Invoke-DockerRestart }
    default { Log-Message "Invalid option: $Option" }
}
