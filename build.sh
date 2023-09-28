
# dependency libraries and root paths

export PAPI_PATH=/apps/PAPI/5.6.0
#export MPI=/apps/OPENMPI/4.1.0/GCC
#export MPI_PATH=/apps/INTEL/2017.4/impi/2017.3.196
#export MPI_PATH=/apps/OPENMPI/1.10.7/GCC/7.1.0
export MPI_PATH=/apps/OPENMPI/4.0.1/GCC
#export EXTRAE=/apps/BSCTOOLS/extrae/4.0.1/openmpi_4_0_1
#export EXTRAE_PATH=/apps/BSCTOOLS/extrae/4.0.1/impi_2017_4
export EXTRAE_PATH=/apps/BSCTOOLS/extrae/4.0.1/openmpi_4_0_1
#export DRAMSIM_PATH=/home/bsc18/bsc18186/scratch/openmpi-1.4.1
#export PIN=/apps/PIN/3.15-98253-gb56e429b1-gcc-linux
export DYNAMORIO_PATH=/gpfs/scratch/bsc18/bsc18292/romol/DR/dynamorio/build
export RAMULATOR_PATH=/gpfs/scratch/bsc18/bsc18292/romol/ramulator/ramulator

# Directories
export ROOT_DIR=/home/bsc18/bsc18186/scratch/musa.3.1
export SRC_DIR=${ROOT_DIR}/src
export BUILD_DIR=${ROOT_DIR}/build
export INSTALL_DIR=${ROOT_DIR}/install

# software versions to use
TASKSIM=tasksim-3.1
NANOX=nanox-0.15
MCXX=mcxx-2.3.0
DIMEMAS=dimemas-5.2.12_musa

# what to build
build_tasksim=true
build_nanox=true
build_mcxx=true
build_dimemas=true

#builr TASKSIM
if $build_tasksim || $build_nanox; then
#if $build_tasksim; then
	mkdir -p ${BUILD_DIR}/${TASKSIM}
	mkdir -p ${INSTALL_DIR}/${TASKSIM}
	echo "cd ${SRC_DIR}/${TASKSIM}"
	cd ${SRC_DIR}/${TASKSIM}
	./setup.sh
	echo "cd ${BUILD_DIR}/${TASKSIM}"
	cd ${BUILD_DIR}/${TASKSIM}

	${SRC_DIR}/${TASKSIM}/configure --prefix=${INSTALL_DIR}/${TASKSIM} \
																	--enable-debug \
																	--enable-nanox \
																	--with-nanox-src-path=${SRC_DIR}/${NANOX} \
																	--with-nanox-install-path=${INSTALL_DIR}/${NANOX} \
																	--enable-mpi \
																	--with-mpi-install-path=${MPI_PATH} \
																	--with-dr-path=${DYNAMORIO_PATH} \
																	--with-extrae-install-path=${EXTRAE_PATH} \
																	--with-ramulator-src-path=${RAMULATOR_PATH} \
																	--with-ramulator-lib-path=${RAMULATOR_PATH} \
																	--enable-compressed-traces \
																	--enable-timestats traceformat=rle \
																	--enable-debug \
																	CFLAGS='-fexceptions' \
																	CXXFLAGS='-fexceptions'


	if [ $? -eq 0 ]; then 
		  echo "Tasksim configure was successful!" 
	else 
		  echo "Tasksim configure failed!"
			exit
		fi
fi
#build NANOX
if $build_nanox; then
	mkdir -p ${BUILD_DIR}/${NANOX}
	mkdir -p ${INSTALL_DIR}/${NANOX}
	echo "cd ${SRC_DIR}/${NANOX}"
	cd ${SRC_DIR}/${NANOX}
	autoreconf -fis
	echo "cd ${BUILD_DIR}/${NANOX}"
	cd ${BUILD_DIR}/${NANOX}
	${SRC_DIR}/${NANOX}/configure	--prefix=${INSTALL_DIR}/${NANOX} \
																		--with-mpi=${MPI_PATH} \
																		--with-mpi-include-path=${MPI_PATH}/include \
																		--with-mpi-lib-path=${MPI_PATH}/lib \
																		--with-nextsim=${BUILD_DIR}/${TASKSIM} \
																		--with-extrae=${EXTRAE_PATH} \
																		--disable-allocator \
																		--disable-gpu-arch \
																		--disable-opencl-arch
					#													CFLAGS='-fexceptions -Wno-error=deprecated-copy' \
					#													CXXFLAGS='-fexceptions -Wno-error=deprecated-copy'
#																		--disable-debug

	if [ $? -eq 0 ]; then 
		  echo "Nanox configure was successful!" 
	else 
		  echo "Nanox configure failed!"
			exit
		fi

	make V=1 && make install

	if [ $? -eq 0 ]; then 
		  echo "Nanox make was successful!" 
	else 
		  echo "Nanox make failed!"
			exit
		fi

fi

#build TASKSIM
if $build_tasksim; then
	echo "cd ${BUILD_DIR}/${TASKSIM}"
	cd ${BUILD_DIR}/${TASKSIM}
	make V=1 && make install

	cd ${ROOT_DIR}
	rm ${INSTALL_DIR}/tasksim
	ls -al ${INSTALL_DIR}
	ln -s ${INSTALL_DIR}/${TASKSIM} ${INSTALL_DIR}/tasksim
fi

#build MCXX
if $build_mcxx; then

	mkdir -p ${BUILD_DIR}/${MCXX}
	mkdir -p ${INSTALL_DIR}/${MCXX}

	echo "cd ${SRC_DIR}/${MCXX}"
	cd ${SRC_DIR}/${MCXX}
	autoreconf -fis
	echo "cd ${BUILD_DIR}/${MCXX}"
	cd ${BUILD_DIR}/${MCXX}
	${SRC_DIR}/${MCXX}/configure --prefix=${INSTALL_DIR}/${MCXX} \
															--with-nanox=${INSTALL_DIR}/${NANOX} \
															--enable-ompss 
	make && make install
fi

#build DIMEMAS
if $build_dimemas; then

	mkdir -p ${BUILD_DIR}/${DIMEMAS}
	mkdir -p ${INSTALL_DIR}/${DIMEMAS}

	echo "cd ${SRC_DIR}/${TASKSIM}/ext/${DIMEMAS}"
	cd ${SRC_DIR}/${TASKSIM}/ext/${DIMEMAS}
	autoreconf -fis
	echo "cd ${BUILD_DIR}/${DIMEMAS}"
	cd ${BUILD_DIR}/${DIMEMAS}
  ${SRC_DIR}/${TASKSIM}/ext/${DIMEMAS}/configure --prefix=${INSTALL_DIR}/${DIMEMAS}
	make && make install
fi

