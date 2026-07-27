import express from 'express';
import cors from 'cors';
import { MongoClient } from 'mongodb';
import dotenv from 'dotenv';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

dotenv.config();

const app = express();
const PORT = process.env.PORT || 5000;
const MONGODB_URI = process.env.MONGODB_URI || 'mongodb+srv://aonikyadavcs23_db_user:Eventra-CampuTix@cluster0.nbrl6dn.mongodb.net/dynamic_island?retryWrites=true&w=majority&appName=Cluster0';
const DB_NAME = process.env.DB_NAME || 'dynamic_island';
const COLLECTION_NAME = process.env.COLLECTION_NAME || 'di_users';

app.use(cors());
app.use(express.json());

// Serve built static frontend files from 'dist' directory
app.use(express.static(path.join(__dirname, 'dist')));

let mongoCollection = null;

// Connect to MongoDB immediately on startup
async function initDatabase() {
  try {
    const client = new MongoClient(MONGODB_URI);
    await client.connect();
    mongoCollection = client.db(DB_NAME).collection(COLLECTION_NAME);
    console.log('\n==================================================');
    console.log('✅ EVENTRA MONGODB CLUSTER CONNECTED SUCCESSFULLY!');
    console.log(`📊 Database: ${DB_NAME}`);
    console.log(`📁 Collection: ${COLLECTION_NAME}`);
    console.log('==================================================\n');
  } catch (error) {
    console.error('\n❌ MONGODB CONNECTION ERROR:', error.message);
  }
}

// POST /api/register - Save user details into "di_users" collection under "dynamic_island" database
app.post('/api/register', async (req, res) => {
  try {
    const { fullName, email, profession, collegeName, yearOfStudy } = req.body;

    if (!fullName || !email || !profession) {
      return res.status(400).json({ success: false, message: 'Full name, email, and profession are required.' });
    }

    if (profession === 'Student' && (!collegeName || !yearOfStudy)) {
      return res.status(400).json({ success: false, message: 'College name and year of study are required for students.' });
    }

    if (!mongoCollection) {
      await initDatabase();
    }

    const userDoc = {
      fullName: String(fullName || '').trim(),
      email: String(email || '').trim().toLowerCase(),
      profession: String(profession || 'Other').trim(),
      collegeName: (profession === 'Student' && collegeName) ? String(collegeName).trim() : 'N/A',
      yearOfStudy: (profession === 'Student' && yearOfStudy) ? String(yearOfStudy).trim() : 'N/A',
      source: 'Dynamic-Island-for-Windows',
      app: 'Dynamic-Island-for-Windows',
      createdAt: new Date(),
    };

    const result = await mongoCollection.insertOne(userDoc);
    console.log(`[MongoDB] Recorded new user: ${userDoc.fullName} (${userDoc.email} | ${userDoc.profession}) -> ID: ${result.insertedId}`);

    return res.status(200).json({
      success: true,
      message: 'Registration saved successfully in Eventra MongoDB (dynamic_island -> di_users).',
      id: result.insertedId,
    });
  } catch (error) {
    console.error('Error saving to Eventra MongoDB:', error);
    return res.status(500).json({ success: false, message: 'Failed to save registration to Eventra MongoDB database.', error: error.message });
  }
});

app.get('/health', (req, res) => {
  res.json({ status: 'OK', company: 'Eventra (CampuTix)', database: DB_NAME, collection: COLLECTION_NAME, connected: !!mongoCollection });
});

// SPA Fallback Middleware: Serve index.html for all non-API paths
app.use((req, res, next) => {
  if (req.method === 'GET' && !req.path.startsWith('/api')) {
    return res.sendFile(path.join(__dirname, 'dist', 'index.html'));
  }
  next();
});

app.listen(PORT, async () => {
  console.log(`Eventra Dynamic Island Server running on http://localhost:${PORT}`);
  await initDatabase();
});
