#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	int number_of_satellite = 4;
	int number_of_log = 50;
	int location_radius = 230;
	ofColor color(39);

	for (int param = 0; param < 360; param += 120) {

		auto base_location = glm::vec3(location_radius * cos((ofGetFrameNum() + param + number_of_log) * DEG_TO_RAD), location_radius * sin((ofGetFrameNum() + param + number_of_log) * DEG_TO_RAD), 0);
		auto base_radius = 50;

		ofFill();
		ofSetColor(color);
		ofDrawSphere(base_location, base_radius * 0.25);

		for (int i = 0; i < number_of_satellite; i++) {

			float noise_seed_x = ofRandom(1000);
			float noise_seed_y = ofRandom(1000);
			auto location = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
			location = glm::normalize(location) * base_radius * 1.5;
			vector<glm::vec3> log_list;
			for (int k = 0; k < number_of_log; k++) {

				auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_y, (ofGetFrameNum() + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
				auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_x, (ofGetFrameNum() + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
				auto tmp_base_location = glm::vec3(location_radius * cos((ofGetFrameNum() + param + k) * DEG_TO_RAD), location_radius * sin((ofGetFrameNum() + param + k) * DEG_TO_RAD), 0);
				auto log_location = tmp_base_location + glm::vec4(location, 0) * rotation_y * rotation_x;
				log_list.push_back(log_location);
			}

			auto head_size = 8;
			ofMesh mesh;
			vector<glm::vec3> right, left;
			glm::vec3 last_location;
			float last_theta;

			for (int k = 0; k < log_list.size() - 1; k++) {

				auto location = log_list[k];
				auto next = log_list[k + 1];

				auto direction = next - location;
				auto theta = atan2(direction.y, direction.x);

				right.push_back(location + glm::vec3(ofMap(k, 0, log_list.size(), 0, head_size) * cos(theta + PI * 0.5), ofMap(k, 0, log_list.size(), 0, head_size) * sin(theta + PI * 0.5), 0));
				left.push_back(location + glm::vec3(ofMap(k, 0, log_list.size(), 0, head_size) * cos(theta - PI * 0.5), ofMap(k, 0, log_list.size(), 0, head_size) * sin(theta - PI * 0.5), 0));

				last_location = location;
				last_theta = theta;
			}

			for (int k = 0; k < right.size(); k++) {

				mesh.addVertex(left[k]);
				mesh.addVertex(right[k]);

				mesh.addColor(ofColor(color, ofMap(k, 0, log_list.size(), 0, 255)));
				mesh.addColor(ofColor(color, ofMap(k, 0, log_list.size(), 0, 255)));
			}

			for (int k = 0; k < mesh.getNumVertices() - 2; k += 2) {

				mesh.addIndex(k + 0); mesh.addIndex(k + 1); mesh.addIndex(k + 3);
				mesh.addIndex(k + 0); mesh.addIndex(k + 2); mesh.addIndex(k + 3);
			}

			mesh.addVertex(last_location);
			mesh.addColor(ofColor(color));
			int index = mesh.getNumVertices();
			for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

				mesh.addVertex(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
				mesh.addColor(ofColor(color));
			}

			for (int k = index; k < mesh.getNumVertices() - 1; k++) {

				mesh.addIndex(index); mesh.addIndex(k + 0); mesh.addIndex(k + 1);
			}

			mesh.draw();
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}