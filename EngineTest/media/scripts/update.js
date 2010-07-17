processInput();
updateLights();

function updateLights()
{
	var x = Math.sin(currentTime / 1000);
	var y = Math.cos(currentTime / 1000);

	objectStore.getObject('BlueLight').position = new QVector3D(x * 100,y * 100, 0);
}

function processInput()
{
	var pos = camera.position;
	var dir = camera.direction;
	var right = camera.right;
	if(keyboard.isPressed(Qt.Key_W))
	{
	//	pos += dir;
		pos.setX(pos.x() + dir.x());
		pos.setY(pos.y() + dir.y());
		pos.setZ(pos.z() + dir.z());
	}
	if(keyboard.isPressed(Qt.Key_S))
	{
		pos.setX(pos.x() - dir.x());
		pos.setY(pos.y() - dir.y());
		pos.setZ(pos.z() - dir.z());
	}
	if(keyboard.isPressed(Qt.Key_A))
	{
		pos.setX(pos.x() - right.x());
		pos.setY(pos.y() - right.y());
		pos.setZ(pos.z() - right.z());
	}
	if(keyboard.isPressed(Qt.Key_D))
	{
		pos.setX(pos.x() + right.x());
		pos.setY(pos.y() + right.y());
		pos.setZ(pos.z() + right.z());
	}
	camera.position = pos;
	if(mouse.isPressed(Qt.RightButton))
	{
		mouseDelta = mouse.computeDelta();
		camera.yaw(new OgreRadian(-mouseDelta.x * 0.01));
	}
	if(mouse.isPressed(Qt.RightButton))
	{
		mouseDelta = mouse.computeDelta();
		camera.pitch(new OgreRadian(-mouseDelta.y * 0.01));
	}
	wheelDelta = mouse.getWheelDelta();
	//print('wheelDelta = ', wheelDelta);
	fov = camera.getFOVy();
	//print('old fov = ', fov.valueRadians);
	tempFOV = new OgreRadian(-wheelDelta * 0.001);
	fov.valueRadians = fov.valueRadians + tempFOV.valueRadians;
	//print('new fov = ', fov.valueRadians);
	camera.setFOVy(fov);
}