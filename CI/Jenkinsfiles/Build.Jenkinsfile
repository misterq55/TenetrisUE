// Tenetris build pipeline: compile, cook, package and distribute (manual trigger only).
// Jenkins job Script Path: CI/Jenkinsfiles/Build.Jenkinsfile
//
// Nodes: the controller has no executors; 'ue5-win' and 'ue5-mac' agents must define the
// UE_ROOT environment variable (Manage Nodes -> node -> Environment variables).
// Credentials: 'tenetris-github-token' (Secret text), 'tenetris-rclone-conf' (Secret file).
//
// Flow: Prepare pins one commit (and creates a draft release for Shipping), a matrix builds
// that exact commit per platform in parallel (Checkout -> Compile -> Cook & Package -> Upload),
// and a Shipping release is published only after every platform succeeded.

def isShipping() {
    return params.BUILD_TYPE == 'Shipping'
}

def wantsPlatform(String target) {
    return params.PLATFORM == 'Both' || params.PLATFORM.toLowerCase() == target
}

// Checks out the commit resolved in Prepare, so every agent builds the same source even if
// master moves while the build is queued.
def checkoutPinned() {
    checkout([
        $class: 'GitSCM',
        branches: [[name: env.BUILD_SHA]],
        userRemoteConfigs: scm.userRemoteConfigs,
        extensions: scm.extensions,
    ])
}

// Runs a CI script with the engine-bundled Python (standard library only).
def py(String args) {
    if (!env.UE_ROOT) {
        error("UE_ROOT is not set on node '${env.NODE_NAME}'.")
    }
    if (isUnix()) {
        sh "\"${env.UE_ROOT}/Engine/Binaries/ThirdParty/Python3/Mac/bin/python3\" -u ${args}"
    } else {
        bat "\"${env.UE_ROOT}\\Engine\\Binaries\\ThirdParty\\Python3\\Win64\\python.exe\" -u ${args}"
    }
}

def withGitHub(Closure body) {
    withCredentials([string(credentialsId: 'tenetris-github-token', variable: 'GH_TOKEN')]) {
        body()
    }
}

def withDrive(Closure body) {
    withCredentials([file(credentialsId: 'tenetris-rclone-conf', variable: 'RCLONE_CONFIG')]) {
        body()
    }
}

def buildArgs() {
    return "--build-type ${params.BUILD_TYPE} --build-number ${env.BUILD_NUMBER}"
}

def upload() {
    if (isShipping()) {
        withGitHub { py("CI/Scripts/publish.py release-upload --tag ${env.RELEASE_TAG}") }
    }
    // Dev: the build itself. Shipping: its private symbols.
    withDrive { py('CI/Scripts/publish.py drive-upload') }
}

// Notification hook; wire Discord (or anything else) here later.
def notify(String result) {
    echo "[notify] ${env.JOB_NAME} #${env.BUILD_NUMBER} ${params.BUILD_TYPE}/${params.PLATFORM}: ${result}"
}

pipeline {
    agent none

    options {
        disableConcurrentBuilds()
        buildDiscarder(logRotator(numToKeepStr: '20'))
        skipDefaultCheckout()
        timeout(time: 6, unit: 'HOURS')
    }

    parameters {
        choice(name: 'BUILD_TYPE', choices: ['Development', 'Shipping'], description: 'Shipping publishes a GitHub Release.')
        choice(name: 'PLATFORM', choices: ['Both', 'Win', 'Mac'], description: 'Target platform(s).')
    }

    environment {
        RELEASE_TAG = "build-${env.BUILD_NUMBER}"
    }

    stages {
        stage('Prepare') {
            agent { label 'ue5-win || ue5-mac' }
            steps {
                script {
                    def scmVars = checkout scm
                    env.BUILD_SHA = scmVars.GIT_COMMIT

                    currentBuild.displayName = "#${env.BUILD_NUMBER} ${params.BUILD_TYPE} ${params.PLATFORM}"
                    currentBuild.description = env.BUILD_SHA.substring(0, 8)

                    if (isShipping()) {
                        withGitHub {
                            py("CI/Scripts/publish.py release-draft --tag ${env.RELEASE_TAG} --sha ${env.BUILD_SHA}")
                        }
                        env.DRAFT_CREATED = 'true'
                    }
                }
            }
        }

        stage('Build') {
            failFast true
            matrix {
                // Agent labels are 'ue5-<target>'; the PLATFORM parameter picks the cells.
                axes {
                    axis {
                        name 'TARGET'
                        values 'win', 'mac'
                    }
                }
                when {
                    beforeAgent true
                    expression { wantsPlatform(env.TARGET) }
                }
                // One agent for the whole cell: Compile and Cook share its workspace.
                agent { label "ue5-${TARGET}" }
                options { timeout(time: 3, unit: 'HOURS') }
                stages {
                    stage('Checkout') {
                        steps {
                            script { checkoutPinned() }
                        }
                    }
                    stage('Compile') {
                        steps {
                            script { py("CI/Scripts/build.py compile ${buildArgs()}") }
                        }
                    }
                    stage('Cook & Package') {
                        steps {
                            script {
                                py("CI/Scripts/build.py cook ${buildArgs()}")
                                py("CI/Scripts/package.py ${buildArgs()}")
                            }
                        }
                    }
                    stage('Upload') {
                        steps {
                            script { upload() }
                        }
                    }
                }
            }
        }

        stage('Publish Release') {
            when {
                beforeAgent true
                expression { isShipping() }
            }
            agent { label 'ue5-win || ue5-mac' }
            steps {
                script {
                    checkoutPinned()
                    withGitHub {
                        py("CI/Scripts/publish.py release-publish --tag ${env.RELEASE_TAG}")
                    }
                }
            }
        }
    }

    post {
        unsuccessful {
            script {
                // Never leave a half-uploaded draft behind.
                if (env.DRAFT_CREATED == 'true') {
                    node('ue5-win || ue5-mac') {
                        checkoutPinned()
                        withGitHub {
                            py("CI/Scripts/publish.py release-delete --tag ${env.RELEASE_TAG}")
                        }
                    }
                }
            }
        }
        // Runs last, after the draft cleanup above.
        cleanup {
            script { notify(currentBuild.currentResult) }
        }
    }
}
