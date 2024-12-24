#include "BotBody.h"

BotBody* newBotBody() {
	BotBody* bbody = (BotBody*)malloc(sizeof(BotBody));
	if (bbody == NULL) return NULL;

	bbody->obj_type = BOT_BODY;

	bbody->obj = inheriteComp(bbody, BOT_BODY);
	if (bbody->obj == NULL) return NULL;
		build_body_part(bbody);

	bbody->obj->update = bbodyUpdate;
	bbody->obj->render = bbodyRender;

	return bbody;
}

void bbodyUpdate(Component* obj, float deltatime) {
	BotBody* body = cast(obj, BOT_BODY);
	
	compUpdateChild(obj, deltatime);
}

void bbodyRender(Component* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(obj->transform);

	// 設置金屬材質屬性
	GLfloat materialAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat materialDiffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat materialSpecular[] = { 0.50f, 0.77f, 0.77f, 1.0f };
	GLfloat materialShininess[] = { 80.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	// Body
	glPushMatrix();
	glScalef(1.0f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
	{
		// Bottom face (normal: (0, -1, 0))
		glNormal3f(0.0f, -1.0f, 0.0f);
		//glColor3f(.5f, .5f, .5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		// Top face (normal: (0, 1, 0))
		glNormal3f(0.0f, 1.0f, 0.0f);
		//glColor3f(.65f, .65f, .65f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		//glColor3f(.7f, .7f, .7f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		// Front face (normal: (0, 0, 1))
		glNormal3f(0.0f, 0.0f, 1.0f);
		//glColor3f(.65f, .65f, .65f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		//glColor3f(.7f, .7f, .7f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		//glColor3f(.5f, .5f, .5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		// Back face (normal: (0, 0, -1))
		glNormal3f(0.0f, 0.0f, -1.0f);
		//glColor3f(.65f, .65f, .65f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		//glColor3f(.5f, .5f, .5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		//glColor3f(.7f, .7f, .7f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

		// Left face (normal: (1, 0, 0))
		glNormal3f(1.0f, 0.0f, 0.0f);
		//glColor3f(.65f, .65f, .65f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		//glColor3f(.5f, .5f, .5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		//glColor3f(.65f, .65f, .65f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		// Right face (normal: (-1, 0, 0))
		glNormal3f(-1.0f, 0.0f, 0.0f);
		//glColor3f(.7f, .7f, .7f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		//glColor3f(.5f, .5f, .5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	}
	glEnd();
	glPopMatrix();

	// Neck
	glPushMatrix();
	glTranslatef(0.0f, 0.4f, 0.0f);
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	glutSolidCylinder(0.07, 0.3, 10, 10);
	glPopMatrix();

	// Render child components
	compRenderChild(obj);
	glPopMatrix();
}


void build_body_part(BotBody* bbody) {
	// head
	BotHead* head = bbody->bhead = newBotHead();
	if (head == NULL) return NULL;
	Connector* headCon = bbody->headConnector = newConnector(head->obj);
	if (headCon == NULL) return NULL;
	ocPushBack(bbody->obj->child_list, headCon->obj);
	glm_translate(headCon->obj->transform, (vec3) { .0f, .5f, .0f });
	glm_translate(head->obj->transform, (vec3) { .0f, .35f, .0f });
	//printf("head: %d\nheadCon: %d\n", head->comp->id, headCon->comp->id);

	// right forearm
	BotArm* rightFArm = bbody->rightFArm = newBotArm();
	if (rightFArm == NULL) return NULL;
	Connector* rFArmCon = bbody->rightFArmConnector = newConnector(rightFArm->obj);
	if (rFArmCon == NULL) return NULL;
	ocPushBack(bbody->obj->child_list, rFArmCon->obj);
	glm_translate(rFArmCon->obj->transform, (vec3) { .5f, .5f, .0f });
	glm_translate(rightFArm->obj->transform, (vec3) { .15f, .0f, .0f });
	glm_scale(rightFArm->obj->transform, (vec3) { .7f, .8f, .7f });
	//printf("rightFArm: %d\nrFArmCon: %d\n", rightFArm->comp->id, rFArmCon->comp->id);

	// left forearm
	BotArm* leftFArm = bbody->leftFArm = newBotArm();
	if (leftFArm == NULL) return NULL;
	Connector* lFArmCon = bbody->leftFArmConnector = newConnector(leftFArm->obj);
	if (lFArmCon == NULL) return NULL;
	ocPushBack(bbody->obj->child_list, lFArmCon->obj);
	glm_translate(lFArmCon->obj->transform, (vec3) { -.5f, .5f, .0f });
	glm_translate(leftFArm->obj->transform, (vec3) { -.15f, .0f, .0f });
	glm_scale(leftFArm->obj->transform, (vec3) { .7f, .8f, .7f });
	//printf("leftFArm: %d\nlFArmCon: %d\n", leftFArm->comp->id, lFArmCon->comp->id);

	// right arm
	BotArm* rightArm = bbody->rightArm = newBotArm();
	if (rightArm == NULL) return NULL;
	Connector* rArmCon = bbody->rightArmConnector = newConnector(rightArm->obj);
	if (rArmCon == NULL) return NULL;
	rArmCon->bIsVisible = 1;
	ocPushBack(rightFArm->obj->child_list, rArmCon->obj);
	glm_translate(rArmCon->obj->transform, (vec3) { .0f, -.85f, .0f });
	//printf("rightArm: %d\nrArmCon: %d\n", rightArm->comp->id, rArmCon->comp->id);

	// left arm
	BotArm* leftArm = bbody->leftArm = newBotArm();
	if (leftArm == NULL) return NULL;
	Connector* lArmCon = bbody->leftArmConnector = newConnector(leftArm->obj);
	if (lArmCon == NULL) return NULL;
	lArmCon->bIsVisible = 1;
	ocPushBack(leftFArm->obj->child_list, lArmCon->obj);
	glm_translate(lArmCon->obj->transform, (vec3) { .0f, -.85f, .0f });
	//printf("leftArm: %d\nlArmCon: %d\n", leftArm->comp->id, lArmCon->comp->id);

	// right thigh
	BotArm* rThigh = bbody->rightThigh = newBotArm();
	if (rThigh == NULL) return NULL;
	Connector* rThCon = bbody->rightThighConnector = newConnector(rThigh->obj);
	if (rThCon == NULL) return NULL;
	ocPushBack(bbody->obj->child_list, rThCon->obj);
	glm_translate(rThCon->obj->transform, (vec3) { .25f, -.5f, .0f });
	glm_scale(rThigh->obj->transform, (vec3) { .8f, .8f, .8f });
	//printf("rThigh: %d\nrThCon: %d\n", rThigh->comp->id, rThCon->comp->id);
	
	// left thigh
	BotArm* lThigh = bbody->leftThigh = newBotArm();
	if (lThigh == NULL) return NULL;
	Connector* lThCon = bbody->leftThighConnector = newConnector(lThigh->obj);
	if (lThCon == NULL) return NULL;
	ocPushBack(bbody->obj->child_list, lThCon->obj);
	glm_translate(lThCon->obj->transform, (vec3) { -.25f, -.5f, .0f });
	glm_scale(lThigh->obj->transform, (vec3) { .8f, .8f, .8f });
	//printf("lThigh: %d\nlThCon: %d\n", lThigh->comp->id, lThCon->comp->id);

	// right calf
	BotArm* rCalf = bbody->rightCalf = newBotArm();
	if (rCalf == NULL) return NULL;
	Connector* rCaCon = bbody->rightCalfConnector = newConnector(rCalf->obj);
	if (rCaCon == NULL) return NULL;
	rCaCon->bIsVisible = 1;
	ocPushBack(rThigh->obj->child_list, rCaCon->obj);
	glm_translate(rCaCon->obj->transform, (vec3) { .0f, -.8f, .0f });
	//printf("rCalf: %d\nrCaCon: %d\n", rCalf->comp->id, rCaCon->comp->id);
	
	// left calf
	BotArm* lCalf = bbody->leftCalf = newBotArm();
	if (lCalf == NULL) return NULL;
	Connector* lCaCon = bbody->leftCalfConnector = newConnector(lCalf->obj);
	if (lCaCon == NULL) return NULL;
	lCaCon->bIsVisible = 1;
	ocPushBack(lThigh->obj->child_list, lCaCon->obj);
	glm_translate(lCaCon->obj->transform, (vec3) { .0f, -.8f, .0f });
	//printf("rCalf: %d\nrCaCon: %d\n", lCalf->comp->id, lCaCon->comp->id);

}
