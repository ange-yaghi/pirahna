#include <demos.h>

using namespace manta;

void manta_demo::boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;
	RayTracer rayTracer;

	ObjFileLoader boxCityObj;
	bool result = boxCityObj.readObjFile(MODEL_PATH "box_city.obj");

	// Create all materials
	LambertianBSDF lambert;

	PhongDistribution blockCoating;
	blockCoating.setPower((math::real)16000);
	BilayerBSDF blockBSDF;
	blockBSDF.setCoatingDistribution(&blockCoating);
	blockBSDF.setDiffuseMaterial(&lambert);
	blockBSDF.setDiffuse(getColor(0xf1, 0xc4, 0x0f));
	blockBSDF.setSpecularAtNormal(math::loadVector(0.1, 0.1, 0.1));
	SimpleBSDFMaterial *blockMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	blockMaterial->setName("Block");
	blockMaterial->setBSDF(&blockBSDF);

	SimpleBSDFMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setReflectance(math::constants::Zero);
	//outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleBSDFMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(5, 5, 5));
	outdoorTopLightMaterial.setReflectance(math::constants::Zero);
	//outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleBSDFMaterial *groundMaterial = rayTracer.getMaterialManager()->newMaterial<SimpleBSDFMaterial>();
	groundMaterial->setName("Ground");
	groundMaterial->setBSDF(&lambert);

	// Create all scene geometry
	Mesh boxCity;
	boxCity.loadObjFileData(&boxCityObj, rayTracer.getMaterialManager());
	boxCity.setFastIntersectEnabled(false);

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	//outdoorTopLightGeometry.setRadius((math::real)20.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(20, 30.0, -13.5));

	SpherePrimitive groundLightGeometry;
	groundLightGeometry.setRadius((math::real)50000.0 - 1);
	groundLightGeometry.setPosition(math::loadVector(0.0, -50000, 0));

	// Create scene objects
	//SceneObject *smallHouseObject = scene.createSceneObject();
	//smallHouseObject->setGeometry(&smallHouse);
	//smallHouseObject->setMaterial(&wallMaterial);

	Octree octree;
	octree.initialize(100.0, math::loadVector(0, 0, 0));
	octree.analyze(&boxCity, 25);

	KDTree kdtree;
	kdtree.initialize(100.0, math::constants::Zero);
	kdtree.analyze(&boxCity, 4);

	kdtree.writeToObjFile("../../workspace/test_results/box_city_kdtree.obj");

	std::cout << "Scene vertices/faces: " << boxCity.getVertexCount() << "/" << boxCity.getFaceCount() << std::endl;
	std::cout << "Octree faces: " << octree.countFaces() << std::endl;
	std::cout << "Leaf count: " << octree.countLeaves() << std::endl;

	constexpr bool useOctree = true;

	SceneObject *boxCityObject = scene.createSceneObject();
	if (useOctree) boxCityObject->setGeometry(&kdtree);
	else boxCityObject->setGeometry(&boxCity);
	boxCityObject->setDefaultMaterial(blockMaterial);

	SceneObject *outdoorTopLightObject = scene.createSceneObject();
	outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	outdoorTopLightObject->setDefaultMaterial(&outdoorTopLightMaterial);

	math::Vector cameraPos = math::loadVector(15.4473, 4.59977, 13.2961);
	math::Vector target = math::loadVector(2.63987, 3.55547, 2.42282);

	constexpr bool regularCamera = false;

	CameraRayEmitterGroup *group;

	// Create the camera

	math::Vector up = math::loadVector(0.0f, 1.0, 0.0);
	math::Vector dir = math::normalize(math::sub(target, cameraPos));
	up = math::cross(math::cross(dir, up), dir);
	up = math::normalize(up);

	manta::SimpleLens lens;
	lens.initialize();
	lens.setPosition(cameraPos);
	lens.setDirection(dir);
	lens.setUp(up);
	lens.setRadius(1.0);
	lens.setSensorResolutionX(resolutionX);
	lens.setSensorResolutionY(resolutionY);
	lens.setSensorHeight(10.0);
	lens.setSensorWidth(10.0 * (resolutionX / (math::real)resolutionY));
	lens.update();

	RandomSampler sampler;

	if (regularCamera) {
		StandardCameraRayEmitterGroup *camera = new StandardCameraRayEmitterGroup;
		camera->setDirection(dir);
		camera->setPosition(cameraPos);
		camera->setUp(up);
		camera->setPlaneDistance(1.0f);
		camera->setPlaneHeight(1.0f);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&sampler);

		group = camera;
	}
	else {
		math::real lensHeight = 1.0;
		math::real focusDistance = 22.0;

		Aperture *aperture = lens.getAperture();
		aperture->setRadius((math::real)0.18);
		lens.setFocus(focusDistance);

		LensCameraRayEmitterGroup *camera = new LensCameraRayEmitterGroup;
		camera->setDirection(math::normalize(math::sub(target, cameraPos)));
		camera->setPosition(cameraPos);
		camera->setLens(&lens);
		camera->setResolutionX(resolutionX);
		camera->setResolutionY(resolutionY);
		camera->setSampleCount(samplesPerPixel);
		camera->setSampler(&sampler);

		group = camera;
	}

	// Initialize and run the ray tracer
	rayTracer.initialize(1000 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(255, 255, 255));
	//rayTracer.setBackgroundColor(getColor(0.0, 0.0, 0.0));
	//rayTracer.setDeterministicSeedMode(true);
	rayTracer.traceAll(&scene, group);
	//rayTracer.tracePixel(519, 1013, &scene, group);

	// Output the results to a scene buffer
	SceneBuffer sceneBuffer;
	group->fillSceneBuffer(&sceneBuffer);

	// Clean up the camera
	group->destroyRays();
	group->destroyEmitters();
	delete group;

	std::string fname = createUniqueRenderFilename("box_city_demo", samplesPerPixel);
	std::string imageFname = std::string(RENDER_OUTPUT) + "bitmap/" + fname + ".bmp";
	std::string rawFname = std::string(RENDER_OUTPUT) + "raw/" + fname + ".fpm";

	RawFile rawFile;
	rawFile.writeRawFile(rawFname.c_str(), &sceneBuffer);
	editImage(&sceneBuffer, imageFname);

	sceneBuffer.destroy();
	rayTracer.destroy();

	boxCity.destroy();
	boxCityObj.destroy();
	octree.destroy();
	kdtree.destroy();

	std::cout << "Standard allocator memory leaks:     " << StandardAllocator::Global()->getLedger() << ", " << StandardAllocator::Global()->getCurrentUsage() << std::endl;
}
