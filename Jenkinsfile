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
  }
  stages {
    stage('build') {
      agent {
        dockerfile {
          filename './Dockerfile'
          label 'docker && linux'
        }
      }
      stages {
        stage('compile') {
          steps {
            sh "rm -rf ${OUTPUT_DIR}"
            sh "mkdir -p ${BUILD_WRAPPER_OUTPUT}"
            sh "build-wrapper-linux-x86-64 --out-dir ${BUILD_WRAPPER_OUTPUT} g++ -o ${EXECUTABLE} ${CPP_FLAGS} ${SOURCE_DIR}/*.cpp"
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
          image 'sonarsource/sonar-scanner-cli:4.2'
          label 'docker && linux'
          args '--entrypoint ""'
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