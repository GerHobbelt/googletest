// Pipeline definition for the build steps of AR.
//
// Copyright: 2017 Ditto Technologies. All Rights Reserved.
// Author: Frankie Li, Daran He, John Inacay
// TODO - Need to migrate to a standardize Debian package deployment script.

@Library('jenkins-shared-library') _


build_configs = [
    'ubuntu_16_04' : [
        'docker_name' : 'ubuntu16-build-env',
        'docker_file' : 'Dockerfile.xenial',
        'repo'        : '3rdparty-16.04',
        'staging_repo': '3rdparty-16.04-staging',
        'dist'        : 'xenial',
    ],
    'ubuntu_14_04' : [
        'docker_name' : 'ubuntu14-build-env',
        'docker_file' : 'Dockerfile.trusty',
        'repo'        : '3rdparty-14.04',
        'staging_repo': '3rdparty-14.04-staging',
        'dist'        : 'trusty',
    ]
]


node('build && docker') {
    // Set max number of builds to keep to 5.
    properties([[$class: 'BuildDiscarderProperty', strategy: [$class: 'LogRotator', artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '5']]]);

    build_configs.each { target, build_config ->
        git_info = common_pipeline.checkoutRepo(target)

        build(target, build_config)

        publish(target, build_config, git_info)

        cleanUp(target)
    }
}

def build(target, build_config) {
    stage("Prepare Build Env ${target}") {
        // We're checking to see if an old image exists. If so, delete it to
        // reduce total space usage.

        docker.build("${build_config.docker_name}", "-f ${build_config.docker_file} .")

        deleteDockerOutdated()
    }

    stage("Build ${target}") {
        def USER_ID = sh (
            script: 'id -u',
            returnStdout: true
        ).trim()
        def GROUP_ID = sh (
            script: 'id -g',
            returnStdout: true
        ).trim()

        withEnv(['USER_ID=${USER_ID}','GROUP_ID=${GROUP_ID}',
                 'RELEASE_KEYSTORE=keystore.jks',
                 'RELEASE_KEY_ALIAS=demoapp',
                 'RELEASE_STORE_PASSWORD=ditto1',
                 'RELEASE_KEY_PASSWORD=ditto1']) {
            docker.image("${build_config.docker_name}").inside {
                sh('./run_build.sh')
            }
        }
    }

    stage("ArchiveArtifacts ${target}") {
        archiveArtifacts(artifacts: 'build/*.deb')
    }
}

def publish(target, build_config, git_info) {
    stage("Publish ${target}") {
        def repo = git_info.is_release ? build_config.repo : build_config.staging_repo

        withAWS(credentials:'package-uploads') {
            sh("./publish.sh ${repo} ${build_config.dist}")
        }
    }
}

def cleanUp(target) {
    stage("CleanUp ${target}") {
        def current_dir = pwd()
        echo "Cleaning up ${current_dir}"
        deleteDir()
    }
}

def deleteDockerOutdated() {
    // Delete stopped docker containers.
    sh(script: "docker ps -aq --no-trunc | xargs --no-run-if-empty docker rm")
    // Delete dangling docker images.
    sh(script: "docker images -q --filter dangling=true | xargs --no-run-if-empty docker rmi")
}