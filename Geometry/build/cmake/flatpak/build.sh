#!/bin/sh
BASEDIR=$(dirname $0)
ROOTDIR=${BASEDIR}/..
INSTDIR=${BASEDIR}/../install

# Notify the user of the correct script usage
warn() {
    echo "usage: build.sh2 [clean]"
}

# Remove the repo and flatpak build directories
clean() {
    echo "Cleaning flatpak directory"
    rm -rf ${BASEDIR}/repo
    rm -rf ${BASEDIR}/cache
    rm -rf ${ROOTDIR}/repo
    rm -rf ${ROOTDIR}/cache
}

# Create the flatpak installation
build() {
    echo "Building edu.cornell.gdiac.code2.flatpak"

    # Create the flatpak
    flatpak-builder --repo=repo --force-clean cache ${BASEDIR}/edu.cornell.gdiac.code2.yml
    flatpak build-bundle repo ${BASEDIR}/edu.cornell.gdiac.code2.flatpak edu.cornell.gdiac.code2
    rm -rf ${INSTDIR}
    mkdir ${INSTDIR}
    mv ${BASEDIR}/edu.cornell.gdiac.code2.flatpak ${INSTDIR}/

    # Create the Steam shortcut
    echo "#!/bin/sh" >> "${INSTDIR}/GeometryLab.exe"
    echo "flatpak run edu.cornell.gdiac.code2" >> "${INSTDIR}/GeometryLab.exe"
    chmod a+x "${INSTDIR}/GeometryLab.exe"

    # Create the flatpak installer
    echo "#!/bin/sh" >> ${INSTDIR}/install.sh
    echo "BASEDIR=\$(dirname \$0)" >> ${INSTDIR}/install.sh
    echo "flatpak install \${BASEDIR}/edu.cornell.gdiac.code2.flatpak " >> ${INSTDIR}/install.sh
    chmod a+x ${INSTDIR}/install.sh

    # Create the flatpak uninstaller
    echo "#!/bin/sh" >> ${INSTDIR}/uninstall.sh
    echo "flatpak uninstall edu.cornell.gdiac.code2 " >> ${INSTDIR}/uninstall.sh
    chmod a+x ${INSTDIR}/uninstall.sh

    echo "Files written to ${INSTDIR}"
}


# Main script
if [ $# -gt 1 ]
then
    warn
elif [ $# -eq 1 ]
then
    if [ $1 = "clean" ]
    then
        clean
    else
        warn
    fi
else
    build
fi
