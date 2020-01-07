pipeline {
  agent {
    dockerfile {
      filename './Dockerfile'
      label 'docker && linux'
    }
  }
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
      SOURCE = "src/*.cpp"
      OUTPUT_DIR = "out"
      EXECUTABLE = "${OUT_DIR}/rumps-royal-pain"
      CPP_FLAGS = "-std=c++17 -O3 -fwhole-program -march=x86-64 -Wall -Wextra -pedantic"
      BUILD_WRAPPER_OUTPUT = "out/bw"
  }
  stages {
    stage('build') {
      steps {
        sh "rm -rf ${OUTPUT_DIR}"
        sh "mkdir -p ${BUILD_WRAPPER_OUTPUT}"
        sh "build-wrapper-linux-x86-64 --out-dir ${BUILD_WRAPPER_OUTPUT} g++ -o ${EXECUTABLE} ${CPP_FLAGS} ${SOURCE}"
      }
    }
    stage('test') {
      steps {
        sh "chmod +x ${EXECUTABLE}"
        sh "${EXECUTABLE}"
      }
    }
    stage('report') {
      steps {
        echo 'TODO'
      }
    }
    stage('sonar quality gate') {
      agent {
        docker {
          image 'sonarsource/sonar-scanner-cli:4.2'
          label 'docker && linux'
        }
      }
      steps {
        withSonarQubeEnv('sonarqube') {
          withEnv([
            "sonar.branch.name=${env.BRANCH_NAME}",
            "sonar.sources=${SOURCE}",
            "sonar.cfamily.build-wrapper-output=${BUILD_WRAPPER_OUTPUT}"
          ]) {
            sh 'sonar-scanner -X'
          }
        }
        sleep 3
        timeout(time: 1, unit: 'MINUTES') {
          waitForQualityGate abortPipeline: true
        }
      }
    }
  }
}