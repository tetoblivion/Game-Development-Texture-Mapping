#include"pch.h"
#include"mywin2.h"
#include"tseg.h"
#include"renderer.h"
#include"object3d.h"





void face_txt_2::draw(vertice *tv) {

	tviewport vpr = mywin2::backbuffer.getviewport(chRED);

	vertice vvv[3];
	vvv[0] = tv[i[0]];
	vvv[1] = tv[i[1]];
	vvv[2] = tv[i[2]];

	clip(vvv, dval, zclip, vpr.center);

	//-----------------
	double nx;
	double k1x, k2x;

	double ny;
	double k1y, k2y;

	k1x = t[1].x - t[0].x;
	k2x = t[2].x - t[0].x;

	nx = t[0].x;

	k1y = t[1].y - t[0].y;
	k2y = t[2].y - t[0].y;

	ny = t[0].y;
	//----------------

	vector3d P, M, N, S;
	vector3d A, B;
	double  cn, icn;

	zbuftype z;

	P = tv[i[0]];         //tv not vvv
	M = tv[i[1]] - P;
	N = tv[i[2]] - P;

	A = P & N;
	B = M & P;

	S.el[2] = dval;


	auto pre = zbuftype((P * (N & M)*dval));


	int i, kr, kz;
	int tr;

	int s, t;


	for (int y = 0; y < mywin2::backbuffer.outy; y++) {

		double x1 = lr[y].left;
		double x2 = lr[y].right;
		if (x1 != -1.0) {

			kz = int(ceil(x1) + ceil(y)*mywin2::cr.right);

			S.el[0] = ceil(x1 - vpr.center.x);  //S.x
			S.el[1] = ceil(y - vpr.center.y);   //S.y

			kr = int(ceil(y)*(vpr.pitch>>2) + ceil(x1));


			for (i = int(ceil(x1)); i < int(ceil(x2)); i++) {

				cn = (S * (N & M));

				if (cn != 0.0) {

					icn = 1.0 / cn;

					//z = zbuftype( (P * (N & M)*dval) / (S * (N & M)) );
					//z = zbuftype((P * (N & M)) / ((S / dval) * (N & M)));
					//z = zbuftype((P * (N & M)*dval) *icn);
					z = pre *icn;

					if (z<zbuf[kz] && z>zclip) {
						zbuf[kz] = z;

						//3d poly to plane
						//double u = (S * (P & N)) / (S * (N & M));
						//double v = (S * (M & P)) / (S * (N & M));
						double u = (S * (P & N)) *icn;
						double v = (S * (M & P)) *icn;  // u and v are between 0.0 and 1.0

						S.el[0]++;  // S.x++

						//plane to texture, affine
						s = (int)ceil( u * k1x + v * k2x + nx);  // if you dont cast to int, it is posible 
						t = (int)ceil( u * k1y + v * k2y + ny);  // to use the fraction part and do bilinear filtering


						if (s >= 0 && t >= 0 && s < txt->width && t < txt->height) {

							tr = t * txt->pitch + s;

							((int32_t*)vpr.videomem)[kr] = txt->data[tr];
						}
					}
				}

				kr += 1;
				kz++;

			}

		}

	}

}
/**/


//whatever the clip does, we don't subdivide polygon
//3d transform returns 0.0 to 1.0 , so no division in affine
//cutting distance, screen distance
//geometric vectors, algebraic vectors
//one thread
//three muls and one div, much faster then, three divs (icn)





//    (S * (P x N))
//u = -------------
//    (S * (N x M))
//
//    (S * (M x P))
//v = -------------
//    (S * (N x M))





