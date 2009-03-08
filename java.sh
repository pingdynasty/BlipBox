test ! -d build && mkdir build
test ! -d build/classes && mkdir build/classes

CP=build/classes
for nm in lib/*.jar
do CP=$CP:$nm
done
javac -classpath $CP -d build/classes `find src -name '*.java'` && \
java -Djava.library.path=lib -classpath $CP $@
