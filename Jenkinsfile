// Pipeline definition for the build steps of AR.
//
// Copyright: 2017 Ditto Technologies. All Rights Reserved.
// Author: Frankie Li, Daran He, John Inacay

// TODO: Need to migrate to a standardize Debian package deployment script.

properties([[
  $class: 'BuildDiscarderProperty',
  strategy: [
    $class: 'LogRotator',
    artifactDaysToKeepStr: '',
    artifactNumToKeepStr: '',
    daysToKeepStr: '',
    numToKeepStr: '5'
  ]
]]);

@Library('jenkins-shared-library') _

BUILD_CONFIGS = [
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
  BUILD_CONFIGS.each { target, build_config ->
    stage("Checkout ${target}") {
      git_info = common_pipeline.checkoutRepo()
    }

    stage("Prepare Build Env ${target}") {
      buildDockerImage(build_config)
    }
  }
}

stage("Tag and deploy?") {
  if (git_branch.startsWith("release/")) {
    input_result = input(
      message: "User input required",
      parameters: [
        choice(
          name: "Do you want to tag and deploy \"${version_number}\" at hash " +
                " \"${git_commit}\"",
          choices: [
            "skip",
            "tag and deploy RC package",
            "tag and deploy RELEASE package"
          ].join("\n")
        )
      ]
    )
    echo "[i] The answer is ${input_result}"
  } else {
    echo "[i] Not a release branch, skipped"
  }
}

node('build && docker') {
  BUILD_CONFIGS.each { target, build_config ->
    stage("Publishing deb package ${target}") {
      deleteDockerOutdated()

      if (!git_info.branch.startsWith("release/") ||
          input_result.contains("skip"))
        // SET DEV VERSION
        buildInsideDocker(build_config, new_rc_number)
        publishDebToS3(build_config.staging_repo, build_config.dist, git_info)

      } else if (input_result.contains(" RC ")) {
        new_rc_number = calcRcNumber(version_number)
        version_suffix = "rc${new_rc_number}"

        // SET RC VERSION
        buildInsideDocker(build_config, new_rc_number)
        archiveArtifacts(artifacts: 'build/*.deb')

        pushTag("release-v${version_number}rc${new_rc_number}")
        publishDebToS3(build_config.repo, build_config.dist, git_info)

      } else if (input_result.contains(" RELEASE ")) {
        buildInsideDocker(build_config, release=true)
        archiveArtifacts(artifacts: 'build/*.deb')

        pushTag("release-v${version_number}")
        publishDebToS3(build_config.staging_repo, build_config.dist, git_info)

      }
    }
  }

  stage("Clean up") {
    deleteDir(pwd())
  }
}
