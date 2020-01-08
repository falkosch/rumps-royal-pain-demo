pipeline {
  agent none
  options {
    skipStagesAfterUnstable()
    timeout(time: 1, unit: 'HOURS')
  }
  triggers {
    pollSCM('H */15 * * *')
  }
  environment {
    CI = true
    HOME = "${env.WORKSPACE}"
    SOURCE_DIR = 'src'
    OUTPUT_DIR = 'out'
    EXECUTABLE = "${OUTPUT_DIR}/rumps-royal-pain"
    CPP_FLAGS = '-std=c++17 -O3 -fwhole-program -march=x86-64 -Wall -Wextra -pedantic'
    BUILD_WRAPPER_OUTPUT = 'out/bw'
    PLATFORM = 'x86_64'
    GPP = '$(which g++)'
  }
  stages {
    stage('build') {
      agent {
        docker {
          image 'gcc-with-sonar-build-wrapper:latest'
          label 'docker && linux'
        }
      }
      stages {
        stage('compile') {
          steps {
            sh "rm -rf ${OUTPUT_DIR}"
            sh "mkdir -p ${BUILD_WRAPPER_OUTPUT}"
            sh "build-wrapper-linux-x86-64 --out-dir ${BUILD_WRAPPER_OUTPUT} ${GPP} -o ${EXECUTABLE} ${CPP_FLAGS} ${SOURCE_DIR}/*.cpp"
          }
        }
        stage('test') {
          steps {
            sh "chmod +x ${EXECUTABLE}"
            sh "${EXECUTABLE}"
          }
        }
      }
    }
    stage('sonar quality gate') {
      agent {
        docker {
          image 'gcc-with-sonar-scanner-cli:latest'
          label 'docker && linux'
        }
      }
      steps {
        withSonarQubeEnv('sonarqube') {
          sh "sonar-scanner -Dsonar.branch.name=${env.BRANCH_NAME} -Dsonar.sources=${SOURCE_DIR} -Dsonar.cfamily.build-wrapper-output=${BUILD_WRAPPER_OUTPUT}"
        }
        sleep 3
        timeout(time: 1, unit: 'MINUTES') {
          waitForQualityGate abortPipeline: true
        }
      }
    }
  }
}