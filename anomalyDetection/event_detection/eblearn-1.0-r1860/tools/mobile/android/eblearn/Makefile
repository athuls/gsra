keystore=${AKEYSROOT}
keyname=${USER}
ndk=${ANDKROOT}
sdk=${ASDKROOT}
name=eblearn
pkg=com.eblearn
app=${ndk}/apps/${name}
atools=${sdk}/tools
apkname=${name}-unsigned
dapkname=${name}-debug

all: lib
	cd ${app} && ant release

lib:
	cd ${app}/../.. && make APP=${name} APP_OPTIM=release

debug:
	cd ${app}/../.. && make APP=${name} APP_OPTIM=debug V=1
	cd ${app} && ant debug

gdb: debug dinstall
	cd ${ndk} && ${ndk}/ndk-gdb --verbose --project=${app} #--launch=${name}

create: # create project files
	${atools}/android update project -p `pwd`
# ${atools}/android create project --package ${pkg} \
# 	--activity ${name} --target 1 \
# 	--path ${app}

install:
	jarsigner -verbose -keystore ${keystore}/${keyname}.keystore \
	-signedjar ${app}/bin/${name}-signed.apk \
	${app}/bin/${apkname}.apk ${keyname}.keystore
	cp ${app}/bin/${name}-signed.apk ${atools}/
	cd ${atools} && ${atools}/adb install -r ${name}-signed.apk

dinstall: # install debug package
	jarsigner -verbose -keystore ${keystore}/${keyname}.keystore \
	-signedjar ${app}/bin/${name}-signed.apk \
	${app}/bin/${dapkname}.apk ${keyname}.keystore
	cp ${app}/bin/${name}-signed.apk ${atools}/
	cd ${atools} && ${atools}/adb install -r ${name}-signed.apk

uninstall:
	cd ${atools} && ${atools}/adb uninstall ${pkg}

clean:
	cd ${app} && ${ndk}/ndk-build clean
	rm -Rf ${app}/bin/*

log:
	${atools}/adb logcat

key:
	@@echo "Your JAVA_HOME is: ${JAVA_HOME}"
	@@echo "If not set, set it to your java root to use the correct keytool"
	${JAVA_HOME}/bin/keytool -genkey -alias ${keyname}.keystore \
		-keyalg RSA -validity 20000 \
		-keystore ${keystore}/${keyname}.keystore